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
#include "iob_calc.h"

using namespace ns::chrono_literals;

namespace ns {
	namespace impl {
		namespace {
			void sort_data( ns_profile_data_t & profile_data, ns_entries_data_t & glucose_data, ns_treatments_data_t & treatments_data ) {
				std::sort( std::begin( profile_data ), std::end( profile_data ), []( auto const & lhs, auto const & rhs ) {
					return lhs.start_date < rhs.start_date;
				} );
				std::sort( std::begin( glucose_data ), std::end( glucose_data ), []( auto const & lhs, auto const & rhs ) {
					return lhs.timestamp < rhs.timestamp;
				} );
				std::sort( std::begin( treatments_data ), std::end( treatments_data ), []( auto const & lhs, auto const & rhs ) {
					return lhs.timestamp < rhs.timestamp;
				} );
			}



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

			ns::duration_minutes_t get_current_dia( ns_profile_data_t const & profile_data, ns::timestamp_t timestamp ) {
				// Assumes profile_data is sorted in order past to present
				for( auto const & profile: profile_data ) {
					if( profile.start_date <= timestamp ) {
						daw::exception::daw_throw_on_false( profile.store, "Invalid profile data detected" );
						return ns::duration_minutes_t{ static_cast<long int>((*profile.store).Default.dia.count( )*60) };
					}
				}
				throw std::runtime_error( "No current profile found" );
			}

			decltype(auto) ts_to_tod( ns::timestamp_t timestamp ) noexcept {
				auto const ts_tmp = date::floor<std::chrono::minutes>( timestamp );
				auto const ts_midnight = date::floor<date::days>( ts_tmp );
				return date::make_time( ts_tmp - ts_midnight );
			}

			size_t ts_to_5min_bin( ns::timestamp_t timestamp ) noexcept {
				auto const ts = ts_to_tod( timestamp );
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

			auto build_dose_tables( ns_profile_data_t const profile_data, ns_entries_data_t const glucose_data, ns_treatments_data_t const treatments_data ) {
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
			auto autotune_data( ns_profile_data_t profile_data, ns_entries_data_t glucose_data, ns_treatments_data_t treatments_data ) {
				// Ensure data is ordered from past to present
				sort_data( profile_data, glucose_data, treatments_data );
				// Build up current state over
				using treatment_t = typename ns_treatments_data_t::value_type;

				return ns::data::profiles::ns_profiles_t{ };
			}
		}    // namespace anonymous
	}	// namespace impl

	ns::data::profiles::ns_profiles_t autotune_data( boost::string_view nightscout_base_url, ns::timestamp_t const tp_start, ns::timestamp_t const tp_end ) {
		auto const launch_policy = std::launch::async;
		auto profile_data = std::async( launch_policy, ns::ns_get_profiles, nightscout_base_url );
		auto glucose_data = std::async( launch_policy, ns::ns_get_entries, nightscout_base_url, tp_start, tp_end );
		auto treatments_data = std::async( launch_policy, ns::ns_get_treatments, nightscout_base_url, tp_start, tp_end );
		return impl::autotune_data( profile_data.get( ), glucose_data.get( ), treatments_data.get( ) );
	}
}	// namespace ns

