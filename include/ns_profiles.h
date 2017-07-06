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

#pragma once

#include <chrono>
#include <string>
#include <vector>

#include <daw/json/daw_json_link.h>

#include "data_types.h"

namespace ns {
	namespace data {
		namespace profiles {
			struct carb_ratio_element_t : public daw::json::daw_json_link<carb_ratio_element_t> {
				boost::optional<uint32_t> time_as_seconds;
				double value;
				std::string time;

				static void json_link_map( );
			}; // carb_ratio_element_t

			struct sens_element_t : public daw::json::daw_json_link<sens_element_t> {
				boost::optional<uint32_t> time_as_seconds;
				double value;
				std::string time;

				static void json_link_map( );
			}; // sens_element_t

			struct basal_element_t : public daw::json::daw_json_link<basal_element_t> {
				boost::optional<uint32_t> time_as_seconds;
				double value;
				std::string time;

				static void json_link_map( );
			}; // basal_element_t

			struct target_low_element_t : public daw::json::daw_json_link<target_low_element_t> {
				boost::optional<uint32_t> time_as_seconds;
				double value;
				std::string time;

				static void json_link_map( );
				target_low_element_t( );
				target_low_element_t( target_low_element_t const &other );
				target_low_element_t( target_low_element_t &&other );
				~target_low_element_t( );

				target_low_element_t &operator=( target_low_element_t const & ) = default;
				target_low_element_t &operator=( target_low_element_t && ) = default;

			  private:
				void set_links( );
			}; // target_low_element_t

			struct target_high_element_t : public daw::json::daw_json_link<target_high_element_t> {
				boost::optional<uint32_t> time_as_seconds;
				double value;
				std::string time;

				static void json_link_map( );
			}; // target_high_element_t

			struct Default_t : public daw::json::daw_json_link<Default_t> {
				std::string units;
				uint32_t delay;
				std::vector<basal_element_t> basal;
				std::vector<target_low_element_t> target_low;
				std::vector<target_high_element_t> target_high;
				double carbs_hr;
				std::vector<carb_ratio_element_t> carb_ratio;
				std::string timezone;
				ns::duration_hours_t dia;
				std::vector<sens_element_t> sens;
				ns::timestamp_t start_date;

				static void json_link_map( );
			}; // Default_t

			struct store_t : public daw::json::daw_json_link<store_t> {
				Default_t Default;

				static void json_link_map( );
			}; // store_t

			struct ns_profiles_t : public daw::json::daw_json_link<ns_profiles_t> {
				ns::timestamp_t created_at;
				boost::optional<uint64_t> mills;
				std::string _id;
				boost::optional<std::string> default_profile;
				boost::optional<store_t> store;
				ns::timestamp_t start_date;

				static void json_link_map( );
			}; // ns_profiles_t
		}      // namespace profiles
	}          // namespace data
} // namespace ns

