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

#include <sstream>
#include <string>
#include <utility>

#include <daw/json/daw_json_link.h>

#include "glucose_unit.h"

namespace ns {
	namespace {
		glucose_unit & get_static_glucose_unit( ) noexcept {
			static glucose_unit result = glucose_unit::mmol_L;
			return result;
		}
	}	// namespace anonymous

	glucose_unit get_default_glucose_display_unit( ) noexcept {
		return get_static_glucose_unit( );
	}

	void set_default_glucose_display_unit( glucose_unit unit ) noexcept {
		get_static_glucose_unit( ) = unit;
	}

	std::string glucose_t::to_string( glucose_unit unit ) const {
		std::stringstream ss;
		switch( unit ) {
			case glucose_unit::mmol_L: ss << to_mmol_L( value ) << "mmol/L"; break;
			case glucose_unit::mg_dL: ss << value << "mg_dL"; break;
		}
		return ss.str( );
	}

	std::string glucose_t::to_string( ) const {
		return to_string( get_static_glucose_unit( ) );
	}

	std::ostream & operator<<( std::ostream & os, glucose_t const & glucose ) {
		os << glucose.to_string( );
		return os;
	}
}  // namespace ns
 
