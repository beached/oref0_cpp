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
#include <cstdint>
#include <string>
#include <vector>
#include <daw/json/daw_json_link.h>


namespace ns {
	namespace data {
		namespace treatments {
			struct ns_treatments_t: public daw::json::JsonLink<ns_treatments_t> {
				std::string created_at;
				boost::optional<std::string> temp;
				boost::optional<double> insulin;
				std::string eventType;
				boost::optional<uint16_t> carbs;
				std::string _id;
				std::string timestamp;
				boost::optional<int16_t> duration;
				std::string enteredBy;
				boost::optional<double> absolute;
				boost::optional<double> rate;

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
