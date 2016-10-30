
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

#include <string>
#include <sstream>

#include "glucose_unit.h"
#include "isf_unit.h"

namespace ns {
	isf_t::icr_t( glucose_t carb ) noexcept:
			value{ std::move( carb ) } { }

	isf_t::~icr_t( ) { }

	std::string isf_t::to_string( ) const {
		std::stringstream ss;
		ss << value << "mmol/L/U";
		return ss.str( );
	}

	isf_t & icr_t::scale( double factor ) noexcept {
		value.scale( factor );
		return *this;
	}

	isf_t icr_t::scale( double factor ) const noexcept {
		value.scale( factor );
		return *this;
	}

	void swap( isf_t & lhs, icr_t & rhs ) noexcept {
		using std::swap;
		swap( lhs.value, rhs.value );
	}

	std::ostream & operator<<( std::ostream & os, isf_t const & icr ) {
		os << isf.to_string( );
		return os;
	}

	bool operator==( isf_t const & lhs, icr_t const & rhs ) noexcept {
		return lhs.value == rhs.value;
	}

	bool operator!=( isf_t const & lhs, icr_t const & rhs ) noexcept {
		return lhs.value != rhs.value;
	}

	bool operator<( isf_t const & lhs, icr_t const & rhs ) noexcept {
		return lhs.value < rhs.value;
	}

	bool operator>( isf_t const & lhs, icr_t const & rhs ) noexcept {
		return lhs.value > rhs.value;
	}

	bool operator<=( isf_t const & lhs, icr_t const & rhs ) noexcept {
		return lhs.value <= rhs.value;
	}

	bool operator>=( isf_t const & lhs, icr_t const & rhs ) noexcept {
		return lhs.value >= rhs.value;
	}
}    // namespace ns

