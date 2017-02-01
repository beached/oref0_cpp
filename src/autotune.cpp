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

			struct carb_dose_t {
				ns::carb_t value;
				ns::timestamp_t timestamp;
				ns::duration_minutes_t carb_action_duration;

				carb_dose_t( ) = delete;
				~carb_dose_t( ) = default;
				carb_dose_t( carb_dose_t const & ) = default;
				carb_dose_t( carb_dose_t && ) = default;
				carb_dose_t & operator=( carb_dose_t const & ) = default;
				carb_dose_t & operator=( carb_dose_t && ) = default;

				carb_dose_t( ns::carb_t dose, ns::timestamp_t time_of_dose, ns::duration_minutes_t carb_duration ):
						value{ std::move( dose ) },
						timestamp{ std::move( time_of_dose ) },
						carb_action_duration{ std::move( carb_duration ) } { }

				bool can_remove( ns::timestamp_t const & current_timestamp ) const {
					return (timestamp + carb_action_duration) >= current_timestamp;
				}

				ns::carb_t tick( ns::timestamp_t const & current_timestamp ) {
					return ns::calculations::calc_cob( value, carb_action_duration, current_timestamp - timestamp );
				}
			};	// carb_dose_t

			enum class insulin_dose_types_t: uint8_t { basal, correction_bolus, carbohydrate_bolus };

			struct insulin_dose_t {
				ns::insulin_t value;
				insulin_dose_types_t dose_type;
				ns::duration_minutes_t dia;

				insulin_dose_t( ) = delete;
				~insulin_dose_t( ) = default;
				insulin_dose_t( insulin_dose_t const & ) = default;
				insulin_dose_t( insulin_dose_t && ) = default;
				insulin_dose_t & operator=( insulin_dose_t const & ) = default;
				insulin_dose_t & operator=( insulin_dose_t && ) = default;

				insulin_dose_t( ns::insulin_t dose, insulin_dose_types_t doseType, ns::duration_minutes_t DIA ):
						value{ std::move( dose ) },
						dose_type{ std::move( doseType ) },
						dia{ std::move( DIA )} { }

				bool can_remove( ns::timestamp_t const & dose_timestamp, ns::timestamp_t const & current_timestamp ) const {
					return (dose_timestamp + dia) >= current_timestamp;
				}

				ns::insulin_t tick( ns::timestamp_t const & dose_timestamp, ns::timestamp_t const & current_timestamp ) const {
					auto const duration_since_dose = std::chrono::duration_cast<ns::duration_minutes_t>( current_timestamp - dose_timestamp );
					auto const p_now = ns::insulin_on_board_pct( duration_since_dose, dia );
					auto const p_prev =  ns::insulin_on_board_pct( duration_since_dose - 5_mins, dia );
					return value.scale( p_now - p_prev );
				}
			};	// insulin_dose_t
			auto autotune_data( ns_profile_data_t profile_data, ns_entries_data_t glucose_data, ns_treatments_data_t treatments_data ) {
				// Ensure data is ordered from past to present
				sort_data( profile_data, glucose_data, treatments_data );
				// Build up current state over
				using treatment_t = typename ns_treatments_data_t::value_type;
				std::vector<carb_dose_t> carb_doses;
				daw::algorithm::transform_if( treatments_data.begin( ), treatments_data.end( ), std::back_inserter( carb_doses ), []( auto const & item ) {
					return item.carbs;
				}, []( auto const & item ) {
					return carb_dose_t{ *item.carbs, item.timestamp, item.absorption_time ? *item.absorption_time : 180.0_mins };
				});
				return ns::data::profiles::ns_profiles_t{ };
			}
		}    // namespace anonymous
	}	// namespace impl

	ns::data::profiles::ns_profiles_t autotune_data( boost::string_view nightscout_base_url, std::chrono::system_clock::time_point const tp_start, std::chrono::system_clock::time_point const tp_end ) {
		auto const launch_policy = std::launch::async;
		auto profile_data = std::async( launch_policy, ns::ns_get_profiles, nightscout_base_url );
		auto glucose_data = std::async( launch_policy, ns::ns_get_entries, nightscout_base_url, tp_start, tp_end );
		auto treatments_data = std::async( launch_policy, ns::ns_get_treatments, nightscout_base_url, tp_start, tp_end );
		return impl::autotune_data( profile_data.get( ), glucose_data.get( ), treatments_data.get( ) );
	}
}	// namespace ns

