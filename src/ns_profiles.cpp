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
#include "time_json.h"

namespace ns {
	namespace data {
		namespace profiles {
			void carb_ratio_element_t::json_link_map( ) {
				link_json_string_to_integral( "timeAsSeconds", time_as_seconds );
				link_json_string_to_real( "value", value );
				link_json_string( "time", time );
			}

			void sens_element_t::json_link_map( ) {
				link_json_string_to_integral( "timeAsSeconds", time_as_seconds );
				link_json_string_to_real( "value", value );
				link_json_string( "time", time );
			}

			void basal_element_t::json_link_map( ) {
				link_json_string_to_integral( "timeAsSeconds", time_as_seconds );
				link_json_string_to_real( "value", value );
				link_json_string( "time", time );
			}

			void target_low_element_t::json_link_map( ) {
				link_json_string_to_integral( "timeAsSeconds", time_as_seconds );
				link_json_string_to_real( "value", value );
				link_json_string( "time", time );
			}

			void target_high_element_t::json_link_map( ) {
				link_json_string_to_integral( "timeAsSeconds", time_as_seconds );
				link_json_string_to_real( "value", value );
				link_json_string( "time", time );
			}

			void Default_t::json_link_map( ) {
				link_json_string( "units", units );
				link_json_string_to_integral( "delay", delay );
				link_json_object_array( "basal", basal );
				link_json_object_array( "target_low", target_low );
				link_json_object_array( "target_high", target_high );
				link_json_string_to_real( "carbs_hr", carbs_hr );
				link_json_object_array( "carbratio", carb_ratio );
				link_json_string( "timezone", timezone );
				ns::impl::link_real_duration( "dia", this, dia );
				link_json_object_array( "sens", sens );
				link_iso8601_timestamp( "startDate", start_date );
			}

			void store_t::json_link_map( ) {
				link_json_object( "Default", Default );
			}

			void ns_profiles_t::json_link_map( ) {
				link_iso8601_timestamp( "created_at", created_at );
				link_json_string_to_integral( "mills", mills );
				link_json_string( "_id", _id );
				link_string( "defaultProfile", default_profile );
				link_object( "store", store );
				link_iso8601_timestamp( "startDate", start_date );
			}
		}	// namespace profiles
	}	// namespace data
}	// namespace ns
