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

namespace ns {
	namespace data {
		namespace profiles {
			struct carbratio_element_t: public daw::json::JsonLink<carbratio_element_t> {
				boost::optional<uint32_t> timeAsSeconds;
				double value;
				std::string time;

				carbratio_element_t( );
				carbratio_element_t( carbratio_element_t const & other );
				carbratio_element_t( carbratio_element_t && other );
				~carbratio_element_t( );

				carbratio_element_t & operator=( carbratio_element_t const & ) = default;
				carbratio_element_t & operator=( carbratio_element_t && ) = default;
			private:
				void set_links( );
			};	// carbratio_element_t

			struct sens_element_t: public daw::json::JsonLink<sens_element_t> {
				boost::optional<uint32_t> timeAsSeconds;
				double value;
				std::string time;

				sens_element_t( );
				sens_element_t( sens_element_t const & other );
				sens_element_t( sens_element_t && other );
				~sens_element_t( );

				sens_element_t & operator=( sens_element_t const & ) = default;
				sens_element_t & operator=( sens_element_t && ) = default;
			private:
				void set_links( );
			};	// sens_element_t

			struct basal_element_t: public daw::json::JsonLink<basal_element_t> {
				boost::optional<uint32_t> timeAsSeconds;
				double value;
				std::string time;

				basal_element_t( );
				basal_element_t( basal_element_t const & other );
				basal_element_t( basal_element_t && other );
				~basal_element_t( );

				basal_element_t & operator=( basal_element_t const & ) = default;
				basal_element_t & operator=( basal_element_t && ) = default;
			private:
				void set_links( );
			};	// basal_element_t

			struct target_low_element_t: public daw::json::JsonLink<target_low_element_t> {
				boost::optional<uint32_t> timeAsSeconds;
				double value;
				std::string time;

				target_low_element_t( );
				target_low_element_t( target_low_element_t const & other );
				target_low_element_t( target_low_element_t && other );
				~target_low_element_t( );

				target_low_element_t & operator=( target_low_element_t const & ) = default;
				target_low_element_t & operator=( target_low_element_t && ) = default;
			private:
				void set_links( );
			};	// target_low_element_t

			struct target_high_element_t: public daw::json::JsonLink<target_high_element_t> {
				boost::optional<uint32_t> timeAsSeconds;
				double value;
				std::string time;

				target_high_element_t( );
				target_high_element_t( target_high_element_t const & other );
				target_high_element_t( target_high_element_t && other );
				~target_high_element_t( );

				target_high_element_t & operator=( target_high_element_t const & ) = default;
				target_high_element_t & operator=( target_high_element_t && ) = default;
			private:
				void set_links( );
			};	// target_high_element_t

			struct Default_t: public daw::json::JsonLink<Default_t> {
				std::string units;
				uint32_t delay;
				std::vector<basal_element_t> basal;
				std::vector<target_low_element_t> target_low;
				std::vector<target_high_element_t> target_high;
				double carbs_hr;
				std::vector<carbratio_element_t> carbratio;
				std::string timezone;
				double dia;
				std::vector<sens_element_t> sens;
				std::chrono::system_clock::time_point start_date;

				Default_t( );
				Default_t( Default_t const & other );
				Default_t( Default_t && other );
				~Default_t( );

				Default_t & operator=( Default_t const & ) = default;
				Default_t & operator=( Default_t && ) = default;
			private:
				void set_links( );
			};	// Default_t

			struct store_t: public daw::json::JsonLink<store_t> {
				Default_t Default;

				store_t( );
				store_t( store_t const & other );
				store_t( store_t && other );
				~store_t( );

				store_t & operator=( store_t const & ) = default;
				store_t & operator=( store_t && ) = default;
			private:
				void set_links( );
			};	// store_t

			struct ns_profiles_t: public daw::json::JsonLink<ns_profiles_t> {
				std::chrono::system_clock::time_point created_at;
				boost::optional<uint64_t> mills;
				std::string _id;
				boost::optional<std::string> defaultProfile;
				boost::optional<store_t> store;
				std::chrono::system_clock::time_point start_date;

				ns_profiles_t( );
				ns_profiles_t( ns_profiles_t const & other );
				ns_profiles_t( ns_profiles_t && other );
				~ns_profiles_t( );

				ns_profiles_t & operator=( ns_profiles_t const & ) = default;
				ns_profiles_t & operator=( ns_profiles_t && ) = default;
			private:
				void set_links( );
			};	// ns_profiles_t
		}	// namespace profiles
	}	// namespace data
}	// namespace ns
