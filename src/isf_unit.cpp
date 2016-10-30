
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
#include "insulin_unit.h"
#include "isf_unit.h"

namespace ns {
	isf_t::isf_t( glucose_t carb ) noexcept:
			value{ std::move( carb ) } { }

	isf_t::~isf_t( ) { }

	std::string isf_t::to_string( ) const {
		std::stringstream ss;
		ss << value << "mmol/L/U";
		return ss.str( );
	}

	isf_t & isf_t::scale( double factor ) noexcept {
		value.scale( factor );
		return *this;
	}

	isf_t isf_t::scale( double factor ) const noexcept {
		isf_t result{ *this };
		result.scale( factor );
		return result;
	}

	void swap( isf_t & lhs, isf_t & rhs ) noexcept {
		using std::swap;
		swap( lhs.value, rhs.value );
	}

	std::ostream & operator<<( std::ostream & os, isf_t const & isf ) {
		os << isf.to_string( );
		return os;
	}

	insulin_t operator/( glucose_t const & lhs, isf_t const & rhs ) noexcept {
		return insulin_t{ lhs.value/rhs.value.value };
	}

	glucose_t operator*( insulin_t const & lhs, isf_t const & rhs ) noexcept {
		return glucose_t{ lhs.value*rhs.value.value };
	}

	glucose_t operator*( isf_t const & lhs, insulin_t const & rhs ) noexcept {
		return glucose_t{ lhs.value.value*rhs.value };
	}

	bool operator==( isf_t const & lhs, isf_t const & rhs ) noexcept {
		return lhs.value == rhs.value;
	}

	bool operator!=( isf_t const & lhs, isf_t const & rhs ) noexcept {
		return lhs.value != rhs.value;
	}

	bool operator<( isf_t const & lhs, isf_t const & rhs ) noexcept {
		return lhs.value < rhs.value;
	}

	bool operator>( isf_t const & lhs, isf_t const & rhs ) noexcept {
		return lhs.value > rhs.value;
	}

	bool operator<=( isf_t const & lhs, isf_t const & rhs ) noexcept {
		return lhs.value <= rhs.value;
	}

	bool operator>=( isf_t const & lhs, isf_t const & rhs ) noexcept {
		return lhs.value >= rhs.value;
	}
}    // namespace ns

