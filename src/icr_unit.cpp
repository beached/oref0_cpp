
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

#include "carb_unit.h"
#include "insulin_unit.h"
#include "icr_unit.h"

namespace ns {
	icr_t::icr_t( carb_t carb ) noexcept:
			value{ std::move( carb ) } { }

	icr_t::~icr_t( ) { }

	std::string icr_t::to_string( ) const {
		std::stringstream ss;
		ss << value << "/U";
		return ss.str( );
	}

	icr_t & icr_t::scale( double factor ) noexcept {
		value.scale( factor );
		return *this;
	}

	icr_t icr_t::scale( double factor ) const noexcept {
		value.scale( factor );
		return *this;
	}

	void swap( icr_t & lhs, icr_t & rhs ) noexcept {
		using std::swap;
		swap( lhs.value, rhs.value );
	}

	std::ostream & operator<<( std::ostream & os, icr_t const & icr ) {
		os << icr.to_string( );
		return os;
	}

	insulin_t operator/( carb_t const & lhs, icr_t const & rhs ) noexcept {
		return insulin_t{ lhs.value/rhs.value.value };
	}

	carb_t operator*( icr_t const & lhs, insulin_t const & rhs ) noexcept {
		return carb_t{ lhs.value.value*rhs.value };
	}

	carb_t operator*( insulin_t const & lhs, icr_t const & rhs ) noexcept {
		return carb_t{ rhs.value.value*lhs.value };
	}

	bool operator==( icr_t const & lhs, icr_t const & rhs ) noexcept {
		return lhs.value == rhs.value;
	}

	bool operator!=( icr_t const & lhs, icr_t const & rhs ) noexcept {
		return lhs.value != rhs.value;
	}

	bool operator<( icr_t const & lhs, icr_t const & rhs ) noexcept {
		return lhs.value < rhs.value;
	}

	bool operator>( icr_t const & lhs, icr_t const & rhs ) noexcept {
		return lhs.value > rhs.value;
	}

	bool operator<=( icr_t const & lhs, icr_t const & rhs ) noexcept {
		return lhs.value <= rhs.value;
	}

	bool operator>=( icr_t const & lhs, icr_t const & rhs ) noexcept {
		return lhs.value >= rhs.value;
	}
}    // namespace ns

