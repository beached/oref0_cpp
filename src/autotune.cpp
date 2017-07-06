// The MIT License (MIT)
//
// Copyright (c) 2017 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <chrono>
#include <date/date.h>
#include <future>
#include <iterator>

#include <daw/daw_algorithm.h>
#include <daw/daw_exception.h>
#include <daw/curl_wrapper.h>

#include "autotune.h"
#include "diabetic_calcs.h"
#include "glucose_unit.h"
#include "glucose_time_unit.h"
#include "iob_calc.h"
#include "isf_unit.h"
#include "icr_unit.h"

using namespace ns::chrono_literals;
namespace std {
	template<>
	struct hash<date::year_month_day> {
		size_t operator()( date::year_month_day const & value ) const {
			using namespace date;
			using namespace std::chrono;
			auto const days_since_epoch = static_cast<sys_days>(value).time_since_epoch( ).count( );
			return hash<int>{ }( days_since_epoch );
		}
	};
}
namespace ns {
	namespace impl {
		namespace {
			struct glucose_reading_t {
				ns::glucose_t value;
				ns::timestamp_t timestamp;

				glucose_reading_t( ) = delete;
				~glucose_reading_t( ) = default;
				glucose_reading_t( glucose_reading_t const & ) = default;
				glucose_reading_t( glucose_reading_t && ) = default;
				glucose_reading_t & operator=( glucose_reading_t const & ) = default;
				glucose_reading_t & operator=( glucose_reading_t && ) = default;

				glucose_reading_t( ns::glucose_t reading, ns::timestamp_t time_of_reading ):
						value{ std::move( reading ) },
						timestamp{ std::move( time_of_reading ) } { }
			};	// glucose_reading_t

			struct carb_dose_t {
				ns::carb_t value;
				ns::duration_minutes_t carb_action_duration;
				bool has_value;
				ns::timestamp_t timestamp;

				carb_dose_t( ) = delete;
				~carb_dose_t( ) = default;
				carb_dose_t( carb_dose_t const & ) = default;
				carb_dose_t( carb_dose_t && ) = default;
				carb_dose_t & operator=( carb_dose_t const & ) = default;
				carb_dose_t & operator=( carb_dose_t && ) = default;

				carb_dose_t( ns::timestamp_t time_of_dose ):
						value{ 0 },
						carb_action_duration{ 0 },
						has_value{ false },
						timestamp{ std::move( time_of_dose ) } { }

				carb_dose_t( ns::carb_t dose, ns::timestamp_t time_of_dose, ns::duration_minutes_t carb_duration ):
						value{ std::move( dose ) },
						carb_action_duration{ std::move( carb_duration ) },
						has_value{ true },
						timestamp{ std::move( time_of_dose ) } { }

				bool can_remove( ns::timestamp_t const & current_timestamp ) const {
					return (timestamp + carb_action_duration) >= current_timestamp;
				}

				ns::carb_t tick( ns::timestamp_t const & current_timestamp ) {
					return ns::calculations::calc_cob( value, carb_action_duration, std::chrono::duration_cast<ns::duration_minutes_t>( current_timestamp - timestamp ) );
				}
			};	// carb_dose_t

			struct insulin_dose_t {
				ns::insulin_t value;
				ns::data::treatments::event_type_t dose_type;
				ns::duration_minutes_t dia;
				bool has_value;
				ns::timestamp_t timestamp;

				insulin_dose_t( ) = delete;

				insulin_dose_t( ns::timestamp_t dose_timestamp ):
						value{ 0 },
						dose_type{ ns::data::treatments::event_type_t::meal_bolus },
						dia{ 0 },
						has_value{ false },
						timestamp{ std::move( dose_timestamp ) } { }

				~insulin_dose_t( ) = default;
				insulin_dose_t( insulin_dose_t const & ) = default;
				insulin_dose_t( insulin_dose_t && ) = default;
				insulin_dose_t & operator=( insulin_dose_t const & ) = default;
				insulin_dose_t & operator=( insulin_dose_t && ) = default;

				insulin_dose_t( ns::insulin_t dose, ns::data::treatments::event_type_t doseType, ns::duration_minutes_t DIA, ns::timestamp_t dose_timestamp ):
						value{ std::move( dose ) },
						dose_type{ std::move( doseType ) },
						dia{ std::move( DIA ) },
						has_value{ true },
						timestamp{ std::move( dose_timestamp ) } { }

				bool can_remove( ns::timestamp_t const & current_timestamp ) const {
					return (timestamp + dia) >= current_timestamp;
				}

				ns::insulin_t tick( ns::timestamp_t const & current_timestamp ) const {
					auto const duration_since_dose = std::chrono::duration_cast<ns::duration_minutes_t>( current_timestamp - timestamp );
					auto const p_now = ns::insulin_on_board_pct( duration_since_dose, dia );
					auto const p_prev =  ns::insulin_on_board_pct( duration_since_dose - 5_mins, dia );
					return value.scale( p_now - p_prev );
				}
			};	// insulin_dose_t

			auto const & get_current_profile( ns_profile_data_t const & profile_data, ns::timestamp_t timestamp ) {
				// Assumes profile_data is sorted in order past to present
				for( auto const & profile: profile_data ) {
					if( profile.start_date <= timestamp ) {
						daw::exception::daw_throw_on_false( profile.store, "Invalid profile data detected" );
						return profile.store->Default;
					}
				}
				throw std::runtime_error( "No current profile found" );
			}

			ns::duration_minutes_t get_current_dia( ns_profile_data_t const & profile_data, ns::timestamp_t timestamp ) {
				return ns::duration_minutes_t{ get_current_profile( profile_data, timestamp ).dia.count( )*60 };
			}

			uint16_t to_tod( boost::string_view tod_str ) {
				daw::exception::daw_throw_on_false( tod_str.size( ) == 5, "Time must be of format hh::mm" );
				auto const hours = static_cast<uint16_t>((tod_str[0] - '0')*10 + (tod_str[1] - '0'));
				auto const minutes = static_cast<uint16_t>( (tod_str[3] - '0'*10) + (tod_str[4] - '0') );
				return ((hours*60)+minutes)/5;
			}

			uint16_t ts_to_tod( ns::timestamp_t timestamp ) {
				using namespace std::chrono;
				using namespace date;
				auto const daypoint = floor<days>( timestamp );
				auto const tod = make_time( timestamp - daypoint );
				return (tod.hours( ).count( )*60 + tod.minutes( ).count( ))/5;
			}

			auto ts_to_time( ns::timestamp_t timestamp ) noexcept {
				auto const ts_tmp = date::floor<std::chrono::minutes>( timestamp );
				auto const ts_midnight = date::floor<date::days>( ts_tmp );
				return date::make_time( ts_tmp - ts_midnight );
			}

			auto get_current_basal( ns_profile_data_t const & profile_data, ns::timestamp_t timestamp ) {
				auto const basals = get_current_profile( profile_data, timestamp ).basal;
				daw::exception::daw_throw_on_true( basals.empty( ), "Must have at least one basal rate" );
				auto it_basal = basals.begin( );
				auto last_it_basal = basals.begin( );
				auto const current_tod = ts_to_tod( timestamp );
				while( it_basal != basals.end( ) && to_tod( it_basal->time ) < current_tod ) {
					last_it_basal = it_basal++;
				}
				return ns::insulin_t{ last_it_basal->value };
			}

			auto get_current_isf( ns_profile_data_t const & profile_data, ns::timestamp_t timestamp ) {
				auto const isfs = get_current_profile( profile_data, timestamp ).sens;
				daw::exception::daw_throw_on_true( isfs.empty( ), "Must have at least one isf value" );
				auto it_isf = isfs.begin( );
				auto last_it_isf = isfs.begin( );
				auto const current_tod = ts_to_tod( timestamp );
				while( it_isf != isfs.end( ) && to_tod( it_isf->time ) < current_tod ) {
					last_it_isf = it_isf++;
				}
				return ns::isf_t{ ns::glucose_t{ last_it_isf->value } };
			}

			auto get_current_icr( ns_profile_data_t const & profile_data, ns::timestamp_t timestamp ) {
				auto const icrs = get_current_profile( profile_data, timestamp ).carb_ratio;
				daw::exception::daw_throw_on_true( icrs.empty( ), "Must have at least one icr value" );
				auto it_icr = icrs.begin( );
				auto last_it_icr = icrs.begin( );
				auto const current_tod = ts_to_tod( timestamp );
				while( it_icr != icrs.end( ) && to_tod( it_icr->time ) < current_tod ) {
					last_it_icr = it_icr++;
				}
				return ns::icr_t{ ns::carb_t{ last_it_icr->value } };
			}

			size_t ts_to_5min_bin( ns::timestamp_t timestamp ) noexcept {
				auto const ts = ts_to_time( timestamp );
				size_t const pos = (ts.hours( ).count( )*60 + ts.minutes( ).count( ))/5;
				return pos;
			}

			auto get_current_basal_doses( ns_profile_data_t const & profile_data, ns::timestamp_t timestamp ) {
				daw::exception::daw_throw_on_true( profile_data.empty( ), "no profiles supplied" );
				daw::exception::daw_throw_on_true( timestamp < profile_data.front( ).start_date, "timestamp is before a valid profile" );

				// Assumes profile_data is sorted in order past to present
				auto it_profile = profile_data.begin( );
				while( it_profile != profile_data.end( ) && it_profile->start_date > timestamp && it_profile->store ) {
					++it_profile;
				}

				struct result_t {
					ns::timestamp_t valid_until;
					std::array<ns::insulin_t, 288> basal_doses;

					result_t( ns::timestamp_t ts_valid_until ):
							valid_until{ std::move( ts_valid_until ) },
							basal_doses{ } { }

					bool is_valid( ns::timestamp_t const timestamp ) const {
						return timestamp < valid_until;
					}

					ns::insulin_t get( ns::timestamp_t const timestamp ) const {
						daw::exception::dbg_throw_on_false( is_valid( timestamp ), "Attemp to get an timestamp that is outside the current range" );
						return basal_doses[ts_to_5min_bin( timestamp )];
					}

				} result{ ns::timestamp_t::max( ) };
				daw::exception::daw_throw_on_true( it_profile->store->Default.basal.empty( ), "At least 1 basal rate at midnight must be specified" );
				uint32_t current_time = 0;
				while( current_time < result.basal_doses.size( ) ) {
					auto end_date = std::next( it_profile ) != profile_data.end( ) ? std::next( it_profile )->start_date : ns::timestamp_t::max( );
					for( auto ts = it_profile->start_date; ts != end_date && current_time < result.basal_doses.size( ); ts += 5_mins, ++current_time ) {

					}
				}
				return result;
			}

			auto build_dose_tables( ns_profile_data_t const & profile_data, ns_entries_data_t const & glucose_data, ns_treatments_data_t const & treatments_data ) {
				struct result_t {
					std::vector<carb_dose_t> carb_doses;
					std::vector<insulin_dose_t> insulin_doses;
				} result;

				auto carb_dose = std::back_inserter( result.carb_doses );
				auto insulin_dose = std::back_inserter( result.insulin_doses );

				auto it_treatments = treatments_data.begin( );
				auto it_glucose = glucose_data.begin( );
				// Ensure we don't have data before we can use it.  Should be
				// simpler later.  Maybe reevaluate after completed to see how
				// it goes
				while( it_treatments->timestamp < it_glucose->timestamp ) {
					++it_treatments;
				}
				for( ; it_glucose != glucose_data.end( ); ++it_glucose ) {

				}

				return result;
			}

			auto ts_to_date_tod( ns::timestamp_t ts ) {
				using namespace date;
				using namespace std::chrono;
				struct result_t {
					year_month_day ymd;
					uint16_t tod;
				};
				auto const daypoint = floor<days>( ts );
				auto const tod = make_time( ts - daypoint );
				result_t result{ year_month_day{ daypoint }, static_cast<uint16_t>( (tod.hours( ).count( )*60 + tod.minutes( ).count( ) )/5 ) };
				return result;
			}

			auto ts_to_ymd( ns::timestamp_t ts ) {
				using namespace date;
				using namespace std::chrono;
				auto const daypoint = floor<days>( ts );
				return year_month_day{ daypoint };
			}

			auto calculate_bgi( ns_profile_data_t const & profile_data, ns_treatments_data_t const & treatments_data, ns::timestamp_t tp_start ) {
				using namespace ns::chrono_literals;
				using namespace date;
				using namespace std::chrono;
				struct bgi_day {
					std::array<ns::glucose_t, 288> bgi_carb;
					std::array<ns::glucose_t, 288> bgi_bolus;
					std::array<ns::glucose_t, 288> bgi_basal;
					date::year_month_day datestamp;
				};
				std::unordered_map<date::year_month_day, bgi_day> result;
				struct insulin_dose_t {
					ns::timestamp_t timestamp;
					ns::insulin_t dose;
					ns::duration_minutes_t duration;

					insulin_dose_t( ns::timestamp_t ts, ns::insulin_t insulin_dose, ns::duration_minutes_t dur ):
							timestamp{ std::move( ts ) },
							dose{ std::move( insulin_dose ) },
							duration{ std::move( dur ) } { }
				};

				struct carb_dose_t {
					ns::timestamp_t timestamp;
					ns::carb_t dose;
					ns::duration_minutes_t duration;

					carb_dose_t( ns::timestamp_t ts, ns::carb_t carb_dose, ns::duration_minutes_t dur ):
							timestamp{ std::move( ts ) },
							dose{ std::move( carb_dose ) },
							duration{ std::move( dur ) } { }

				};

				auto const clean_queue = []( auto & queue, auto const ts_now ) {
					auto const should_remove = [&]( auto const & item ) {
						return item.timestamp + item.duration >= ts_now;
					};
					auto const pos = std::remove_if( std::begin( queue ), std::end( queue ), should_remove );
					queue.erase( pos, std::end( queue ) );
				};

				std::vector<insulin_dose_t> basal_queue;
				std::vector<insulin_dose_t> bolus_queue;
				std::vector<carb_dose_t> carb_queue;
				boost::optional<insulin_dose_t> temp_basal;

				for( auto const & cur_treatment: treatments_data ) {
					auto const current_ts = cur_treatment.timestamp;
					auto const current_dia = get_current_dia( profile_data, current_ts );
					if( cur_treatment.carbs ) {
						auto const abs_time = cur_treatment.absorption_time ? *cur_treatment.absorption_time : ns::duration_minutes_t{ 3 };
						carb_queue.emplace_back( current_ts, *cur_treatment.carbs, abs_time );
					}
					if( cur_treatment.insulin ) {
						bolus_queue.emplace_back( current_ts, *cur_treatment.insulin, current_dia );
					}
					if( cur_treatment.absolute ) {
						daw::exception::daw_throw_on_false( cur_treatment.duration, "Temp basal's must have a duration" );
						temp_basal = insulin_dose_t{ cur_treatment.timestamp, cur_treatment.absolute->value, *cur_treatment.duration };
					}
					if( temp_basal && ((temp_basal->timestamp + temp_basal->duration) <= current_ts) ) {
						temp_basal = boost::none;
					}
					if( temp_basal ) {
						auto dose = temp_basal->dose;
						dose.scale( 1.0/12.0 );
						basal_queue.emplace_back( current_ts, dose, current_dia );
					} else {
						auto dose = get_current_basal( profile_data, current_ts );
						dose.scale( 1.0/12.0 );
						basal_queue.emplace_back( current_ts, dose, current_dia );
					}

					clean_queue( carb_queue, current_ts );
					clean_queue( basal_queue, current_ts );
					clean_queue( bolus_queue, current_ts );
					if( cur_treatment.timestamp >= tp_start ) {
						auto const current_tod = ts_to_tod( current_ts );
						auto const current_ymd = ts_to_ymd( current_ts );
						auto const calc_insulin_bgi = []( ns::timestamp_t ts, insulin_dose_t dose, isf_t const & isf ) {
							auto const age = floor<ns::duration_minutes_t>( ts - dose.timestamp );
							if( age > 5_mins ) {
								return 0_mmol_L;
							}
							auto const pc_left = ns::insulin_on_board_pct( age - 5_mins, dose.duration ) - ns::insulin_on_board_pct( age, dose.duration );
							auto insulin_dose = dose.dose;
							insulin_dose.scale( pc_left );
							auto result = isf * insulin_dose;
							return result;
						};
						auto & current_result_day = result[current_ymd];
						current_result_day.datestamp = current_ymd;
						auto const current_isf = get_current_isf( profile_data, current_ts );
						{
							auto & current_basal_tod = current_result_day.bgi_basal[current_tod];
							for( auto const & basal_item: basal_queue ) {
								auto const bgi = calc_insulin_bgi( current_ts, basal_item, current_isf );
								current_basal_tod += bgi;
							}
						}
						{
							auto & current_bolus_tod = current_result_day.bgi_bolus[current_tod];
							for( auto const & bolus_item: bolus_queue ) {
								auto const bgi = calc_insulin_bgi( current_ts, bolus_item, current_isf );
								current_bolus_tod += bgi;
							}
						}
						{
							auto & current_carb_tod = current_result_day.bgi_carb[current_tod];
							auto const current_icr = get_current_icr( profile_data, current_ts );
							for( auto const & carb_item: carb_queue ) {
								auto const age = floor<ns::duration_minutes_t>( current_ts - carb_item.timestamp );
								if( age < 5_mins ) {
									continue;
								}
								auto carb_dose = ns::calculations::calc_cob( carb_item.dose, carb_item.duration, age - 5_mins ) - ns::calculations::calc_cob( carb_item.dose, carb_item.duration, age );
								auto const bgi = (carb_dose / current_icr) * current_isf;
								current_carb_tod += bgi;
							}
						}
					}
				}
				std::vector<bgi_day> vresult;
				std::transform( result.begin( ), result.end( ), std::back_inserter( vresult ), []( auto const & item ) {
					return item.second;
				});
				std::sort( vresult.begin( ), vresult.end( ), []( auto const & lhs, auto const & rhs ) {
					return lhs.datestamp < rhs.datestamp;
				});
				return vresult;
			}

			auto calculate_actual_total_bgi( ns_profile_data_t const & profile_data, ns_entries_data_t const & glucose_data, ns::timestamp_t tp_start  ) {
				using namespace date;
				using namespace std::chrono;
				using namespace ns::chrono_literals;
				struct glucose_day {
					std::array<ns::glucose_t, 288> actual_total_bgi;
					year_month_day datestamp;
				};
				std::unordered_map<year_month_day, glucose_day> glucose_changes;
				daw::exception::daw_throw_on_false( glucose_data.size( ) > 1, "Cannot calculate data with less than 2 values" );
				auto glucose_data_it = std::next( glucose_data.begin( ) );
				auto glucose_data_last_it = glucose_data.begin( );
				while( glucose_data_it != glucose_data.end( ) ) {
					if( glucose_data_it->timestamp >= tp_start && glucose_data_it->sgv > 2.22_mmol_L && glucose_data_last_it->sgv > 2.22_mmol_L ) {
						auto const ymd = ts_to_ymd( glucose_data_it->timestamp );
						auto const tod = ts_to_tod( glucose_data_it->timestamp );
						auto & glucose_day = glucose_changes[ymd];
						glucose_day.datestamp = ymd;
						glucose_day.actual_total_bgi[tod] = glucose_data_it->sgv - glucose_data_last_it->sgv;
					}
					glucose_data_last_it = glucose_data_it;
					++glucose_data_it;
				}
				std::vector<glucose_day> result;
				std::transform( glucose_changes.begin( ), glucose_changes.end( ), std::back_inserter( result ), []( auto const & item ) {
					return item.second;
				});
				std::sort( result.begin( ), result.end( ), []( auto const & lhs, auto const & rhs ) {
					return lhs.datestamp < rhs.datestamp;
				});
				return result;
			}

			namespace bgi {
				enum tags: uint8_t {
					basal = 1,
					bolus = 2,
					carbohydrate = 4
				};
			}

			struct dose_values {
				uint16_t tod;
				insulin_rate_t basal;
				boost::optional<ns::isf_t> isf;
				boost::optional<ns::icr_t> icr;
			};

			void process_bgi( dose_values & results, auto const & bgi_data, ns_entries_data_t const & glucose_data, ns::timestamp_t tp_start ) {

			}

			auto autotune_data( ns_profile_data_t const & profile_data, ns_entries_data_t const & glucose_data, ns_treatments_data_t const & treatments_data, ns::timestamp_t tp_start ) {
				// Assumes data is sorted by timestamps
				// Build up current state over
				using treatment_t = typename ns_treatments_data_t::value_type;
				auto const bgi_data = calculate_bgi( profile_data, treatments_data, tp_start );
				std::array<dose_values, 288> new_doses;
				for( size_t n=0; n<new_doses.size( ); ++n ) {
					new_doses[n].tod = n;
				}

				auto const bgi_tagged_data = process_bgi( 0, bgi_data, glucose_data, tp_start );
				auto actual_bgi = calculate_actual_total_bgi( profile_data, glucose_data, tp_start );
				return ns::data::profiles::ns_profiles_t{ };
			}
		}    // namespace anonymous
	}	// namespace impl

	ns::data::profiles::ns_profiles_t autotune_data( ns::autotune_config_t const & config, ns::timestamp_t const tp_start, ns::timestamp_t const tp_end ) {
		auto const launch_policy = std::launch::async;
		auto profile_data_fut = std::async( launch_policy, ns::ns_get_profiles, config );
		auto glucose_data_fut = std::async( launch_policy, ns::ns_get_entries, config, tp_start, tp_end );
		auto treatments_data_fut = std::async( launch_policy, ns::ns_get_treatments, config, tp_start, tp_end );

		auto const profile_data = profile_data_fut.get( );
		auto const glucose_data = glucose_data_fut.get( );
		auto const treatments_data = treatments_data_fut.get( );
		return impl::autotune_data( profile_data, glucose_data, treatments_data, tp_start );
	}
}	// namespace ns

