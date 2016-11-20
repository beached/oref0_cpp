// The MIT License (MIT)
//
// Copyright (c) 2016 Darrell Wright
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
#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstdint>
#include <string>

#include <daw/json/daw_json_link.h>

namespace ns {
	enum class nightscout_direction: uint8_t { 
			none = 0, 
			double_up = 1, 
			single_up = 2, 
			forty_five_up = 3, 
			flat = 4, 
			forty_five_down = 5, 
			single_down = 6, 
			double_down = 7, 
			not_computable = 8, 
			rate_out_of_range = 9
	};

	std::ostream & operator<<( std::ostream & os, nightscout_direction dir );
	std::istream & operator>>( std::istream & is, nightscout_direction & dir );

	struct nightscout_entries: public daw::json::JsonLink<nightscout_entries> {
		std::string id;
		nightscout_direction direction;
		boost::optional<int16_t> previous_sgv;
		boost::posix_time::ptime timestamp;
		std::string device;
		int64_t date;
		int16_t sgv;
		std::string type;
		boost::optional<bool> previous_sgv_not_active;

		nightscout_entries( );
		~nightscout_entries( );
		nightscout_entries( nightscout_entries const & other );
		nightscout_entries & operator=( nightscout_entries const & rhs );
		
		nightscout_entries( nightscout_entries && ) = default;
		nightscout_entries & operator=( nightscout_entries && ) = default;
	};	// nightscout_entries
}
