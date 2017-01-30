// The MIT License (MIT)
//
// Copyright (c) 2016-2017 Darrell Wright
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

#include <string>
#include <vector>
#include <daw/json/daw_json_link.h>

#include "ns_profiles.h"

namespace ns {
	namespace data {
		namespace profiles {
			carbratio_element_t::carbratio_element_t( ):
				daw::json::JsonLink<carbratio_element_t>{ },
				timeAsSeconds{ },
				value{ },
				time{ } {

					set_links( );
				}

			carbratio_element_t::carbratio_element_t( carbratio_element_t const & other ):
				daw::json::JsonLink<carbratio_element_t>{ },
				timeAsSeconds{ other.timeAsSeconds },
				value{ other.value },
				time{ other.time } {

					set_links( );
				}

			carbratio_element_t::carbratio_element_t( carbratio_element_t && other ):
				daw::json::JsonLink<carbratio_element_t>{ },
				timeAsSeconds{ std::move( other.timeAsSeconds ) },
				value{ std::move( other.value ) },
				time{ std::move( other.time ) } {

					set_links( );
				}

			carbratio_element_t::~carbratio_element_t( ) { }

			void carbratio_element_t::set_links( ) {
				link_json_string_to_integral( "timeAsSeconds", timeAsSeconds );
				link_json_string_to_real( "value", value );
				link_string( "time", time );
			}

			sens_element_t::sens_element_t( ):
				daw::json::JsonLink<sens_element_t>{ },
				timeAsSeconds{ },
				value{ },
				time{ } {

					set_links( );
				}

			sens_element_t::sens_element_t( sens_element_t const & other ):
				daw::json::JsonLink<sens_element_t>{ },
				timeAsSeconds{ other.timeAsSeconds },
				value{ other.value },
				time{ other.time } {

					set_links( );
				}

			sens_element_t::sens_element_t( sens_element_t && other ):
				daw::json::JsonLink<sens_element_t>{ },
				timeAsSeconds{ std::move( other.timeAsSeconds ) },
				value{ std::move( other.value ) },
				time{ std::move( other.time ) } {

					set_links( );
				}

			sens_element_t::~sens_element_t( ) { }

			void sens_element_t::set_links( ) {
				link_json_string_to_integral( "timeAsSeconds", timeAsSeconds );
				link_json_string_to_real( "value", value );
				link_string( "time", time );
			}

			basal_element_t::basal_element_t( ):
				daw::json::JsonLink<basal_element_t>{ },
				timeAsSeconds{ },
				value{ },
				time{ } {

					set_links( );
				}

			basal_element_t::basal_element_t( basal_element_t const & other ):
				daw::json::JsonLink<basal_element_t>{ },
				timeAsSeconds{ other.timeAsSeconds },
				value{ other.value },
				time{ other.time } {

					set_links( );
				}

			basal_element_t::basal_element_t( basal_element_t && other ):
				daw::json::JsonLink<basal_element_t>{ },
				timeAsSeconds{ std::move( other.timeAsSeconds ) },
				value{ std::move( other.value ) },
				time{ std::move( other.time ) } {

					set_links( );
				}

			basal_element_t::~basal_element_t( ) { }

			void basal_element_t::set_links( ) {
				link_json_string_to_integral( "timeAsSeconds", timeAsSeconds );
				link_json_string_to_real( "value", value );
				link_string( "time", time );
			}

			target_low_element_t::target_low_element_t( ):
				daw::json::JsonLink<target_low_element_t>{ },
				timeAsSeconds{ },
				value{ },
				time{ } {

					set_links( );
				}

			target_low_element_t::target_low_element_t( target_low_element_t const & other ):
				daw::json::JsonLink<target_low_element_t>{ },
				timeAsSeconds{ other.timeAsSeconds },
				value{ other.value },
				time{ other.time } {

					set_links( );
				}

			target_low_element_t::target_low_element_t( target_low_element_t && other ):
				daw::json::JsonLink<target_low_element_t>{ },
				timeAsSeconds{ std::move( other.timeAsSeconds ) },
				value{ std::move( other.value ) },
				time{ std::move( other.time ) } {

					set_links( );
				}

			target_low_element_t::~target_low_element_t( ) { }

			void target_low_element_t::set_links( ) {
				link_json_string_to_integral( "timeAsSeconds", timeAsSeconds );
				link_json_string_to_real( "value", value );
				link_string( "time", time );
			}

			target_high_element_t::target_high_element_t( ):
				daw::json::JsonLink<target_high_element_t>{ },
				timeAsSeconds{ },
				value{ },
				time{ } {

					set_links( );
				}

			target_high_element_t::target_high_element_t( target_high_element_t const & other ):
				daw::json::JsonLink<target_high_element_t>{ },
				timeAsSeconds{ other.timeAsSeconds },
				value{ other.value },
				time{ other.time } {

					set_links( );
				}

			target_high_element_t::target_high_element_t( target_high_element_t && other ):
				daw::json::JsonLink<target_high_element_t>{ },
				timeAsSeconds{ std::move( other.timeAsSeconds ) },
				value{ std::move( other.value ) },
				time{ std::move( other.time ) } {

					set_links( );
				}

			target_high_element_t::~target_high_element_t( ) { }

			void target_high_element_t::set_links( ) {
				link_json_string_to_integral( "timeAsSeconds", timeAsSeconds );
				link_json_string_to_real( "value", value );
				link_string( "time", time );
			}

			Default_t::Default_t( ):
				daw::json::JsonLink<Default_t>{ },
				units{ },
				delay{ },
				basal{ },
				target_low{ },
				target_high{ },
				carbs_hr{ },
				carbratio{ },
				timezone{ },
				dia{ },
				sens{ },
				start_date{ } {

					set_links( );
				}

			Default_t::Default_t( Default_t const & other ):
				daw::json::JsonLink<Default_t>{ },
				units{ other.units },
				delay{ other.delay },
				basal{ other.basal },
				target_low{ other.target_low },
				target_high{ other.target_high },
				carbs_hr{ other.carbs_hr },
				carbratio{ other.carbratio },
				timezone{ other.timezone },
				dia{ other.dia },
				sens{ other.sens },
				start_date{ other.start_date } {

					set_links( );
				}

			Default_t::Default_t( Default_t && other ):
				daw::json::JsonLink<Default_t>{ },
				units{ std::move( other.units ) },
				delay{ std::move( other.delay ) },
				basal{ std::move( other.basal ) },
				target_low{ std::move( other.target_low ) },
				target_high{ std::move( other.target_high ) },
				carbs_hr{ std::move( other.carbs_hr ) },
				carbratio{ std::move( other.carbratio ) },
				timezone{ std::move( other.timezone ) },
				dia{ std::move( other.dia ) },
				sens{ std::move( other.sens ) },
				start_date{ std::move( other.start_date ) } {

					set_links( );
				}

			Default_t::~Default_t( ) { }

			void Default_t::set_links( ) {
				link_string( "units", units );
				link_json_string_to_integral( "delay", delay );
				link_array( "basal", basal );
				link_array( "target_low", target_low );
				link_array( "target_high", target_high );
				link_json_string_to_real( "carbs_hr", carbs_hr );
				link_array( "carbratio", carbratio );
				link_string( "timezone", timezone );
				link_json_string_to_real( "dia", dia );
				link_array( "sens", sens );
				link_iso8601_timestamp( "start_date", start_date );
			}

			store_t::store_t( ):
				daw::json::JsonLink<store_t>{ },
				Default{ } {

					set_links( );
				}

			store_t::store_t( store_t const & other ):
				daw::json::JsonLink<store_t>{ },
				Default{ other.Default } {

					set_links( );
				}

			store_t::store_t( store_t && other ):
				daw::json::JsonLink<store_t>{ },
				Default{ std::move( other.Default ) } {

					set_links( );
				}

			store_t::~store_t( ) { }

			void store_t::set_links( ) {
				link_object( "Default", Default );
			}

			ns_profiles_t::ns_profiles_t( ):
				daw::json::JsonLink<ns_profiles_t>{ },
				created_at{ },
				mills{ },
				_id{ },
				defaultProfile{ },
				store{ },
				start_date{ } {

					set_links( );
				}

			ns_profiles_t::ns_profiles_t( ns_profiles_t const & other ):
				daw::json::JsonLink<ns_profiles_t>{ },
				created_at{ other.created_at },
				mills{ other.mills },
				_id{ other._id },
				defaultProfile{ other.defaultProfile },
				store{ other.store },
				start_date{ other.start_date } {

					set_links( );
				}

			ns_profiles_t::ns_profiles_t( ns_profiles_t && other ):
				daw::json::JsonLink<ns_profiles_t>{ },
				created_at{ std::move( other.created_at ) },
				mills{ std::move( other.mills ) },
				_id{ std::move( other._id ) },
				defaultProfile{ std::move( other.defaultProfile ) },
				store{ std::move( other.store ) },
				start_date{ std::move( other.start_date ) } {

					set_links( );
				}

			ns_profiles_t::~ns_profiles_t( ) { }

			void ns_profiles_t::set_links( ) {
				link_iso8601_timestamp( "created_at", created_at );
				link_json_string_to_integral( "mills", mills );
				link_string( "_id", _id );
				link_string( "defaultProfile", defaultProfile );
				link_object( "store", store );
				link_iso8601_timestamp( "startDate", start_date );
			}
		}	// namespace profiles
	}	// namespace data
}	// namespace ns
