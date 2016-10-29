
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

#include <sstream>
#include <string>
#include <utility>

#include "insulin_unit.h"

namespace ns {
	insulin_t::insulin_t( double d ) noexcept:
			value{ std::move( d ) } { }

	insulin_t::operator double( ) noexcept {
		return value;
	}

	insulin_t::~insulin_t( ) { }

	void swap( insulin_t & lhs, insulin_t & rhs ) noexcept {
		using std::swap;
		swap( lhs.value, rhs.value );
	}

	std::string insulin_t::to_string( ) const {
		std::stringstream ss;
		ss << value << "U";
		return ss.str( );
	}

	std::ostream & operator<<( std::ostream & os, insulin_t const & glucose ) {
		os << glucose.to_string( );
		return os;
	}

	insulin_t & insulin_t::operator+=( insulin_t const & rhs ) noexcept {
		value += rhs.value;
		return *this;
	}

	insulin_t & insulin_t::operator-=( insulin_t const & rhs ) noexcept {
		value -= rhs.value;
		return *this;
	}

	insulin_t & insulin_t::scale( double factor ) noexcept {
		value *= factor;
		return *this;
	}

	insulin_t insulin_t::scale( double factor ) const noexcept {
		return insulin_t{ value * factor };
	}

	insulin_t operator+( insulin_t const & lhs, insulin_t const & rhs ) noexcept {
		return insulin_t{ lhs.value + rhs.value };
	}

	insulin_t operator-( insulin_t const & lhs, insulin_t const & rhs ) noexcept {
		return insulin_t{ lhs.value - rhs.value };
	}

	insulin_t operator-( insulin_t rhs ) noexcept {
		rhs.value *= -1.0; 
		return rhs;
	}

	bool operator==( insulin_t const & lhs, insulin_t const & rhs ) noexcept {
		return lhs.value == rhs.value;
	}

	bool operator!=( insulin_t const & lhs, insulin_t const & rhs ) noexcept {
		return lhs.value != rhs.value;
	}

	bool operator<( insulin_t const & lhs, insulin_t const & rhs ) noexcept {
		return lhs.value < rhs.value;
	}

	bool operator>( insulin_t const & lhs, insulin_t const & rhs ) noexcept {
		return lhs.value > rhs.value;
	}

	bool operator<=( insulin_t const & lhs, insulin_t const & rhs ) noexcept {
		return lhs.value <= rhs.value;
	}

	bool operator>=( insulin_t const & lhs, insulin_t const & rhs ) noexcept {
		return lhs.value >= rhs.value;
	}
}    // namespace ns

ns::insulin_t operator"" _U( long double d ) noexcept {
	return ns::insulin_t{ static_cast<double>( d ) };
}

ns::insulin_t operator"" _U( unsigned long long i ) noexcept {
	return ns::insulin_t{ static_cast<double>( i ) };
}

