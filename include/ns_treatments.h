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

#pragma once

#include <boost/optional.hpp>
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <daw/json/daw_json_link.h>

#include "carb_unit.h"
#include "insulin_unit.h"
#include "insulin_time_unit.h"

namespace ns {
	namespace data {
		namespace treatments {
			enum class temp_basal_t: uint8_t { absolute, percentage };
			std::ostream & operator<<( std::ostream & os, temp_basal_t const & tb );
			std::istream & operator>>( std::istream & is, temp_basal_t & tb );
			enum class event_type_t: uint8_t { temp_basal, bg_check, correction_bolus, meal_bolus };
			std::ostream & operator<<( std::ostream & os, event_type_t const & tb );
			std::istream & operator>>( std::istream & is, event_type_t & tb );

			struct ns_treatments_t: public daw::json::JsonLink<ns_treatments_t> {
				std::chrono::system_clock::time_point created_at;
				boost::optional<temp_basal_t> temp;
				boost::optional<ns::insulin_t> insulin;
				boost::optional<std::string> glucose_type;
				event_type_t eventType;
				boost::optional<ns::carb_t> carbs;
				std::string _id;
				std::chrono::system_clock::time_point timestamp;
				boost::optional<std::chrono::minutes> duration;
				std::string enteredBy;
				boost::optional<ns::insulin_rate_t> absolute;
				boost::optional<double> rate;
				boost::optional<double> glucose;
				boost::optional<std::string> units;
				boost::optional<std::chrono::minutes> absorption_time;
				boost::optional<double> programmed;
				boost::optional<double> ratio;
				boost::optional<double> unabsorbed;


				ns_treatments_t( );
				ns_treatments_t( ns_treatments_t const & other );
				ns_treatments_t( ns_treatments_t && other );
				~ns_treatments_t( );

				ns_treatments_t & operator=( ns_treatments_t const & ) = default;
				ns_treatments_t & operator=( ns_treatments_t && ) = default;
				private:
				void set_links( );
			};	// ns_treatments_t
		}	// namespace treatmetns
	}	// namespace data
}	// namespace ns


