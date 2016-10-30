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

#include "glucose_unit.h"

namespace ns {
	namespace {
		constexpr double to_mg_dL( double const & glucose ) noexcept {
			return glucose / 0.0555;
		}

		constexpr double to_mmol_L( double const & glucose ) noexcept {
			return glucose * 0.0555;
		}

		glucose_unit & get_static_glucose_unit( ) {
			static glucose_unit result = glucose_unit::mmol_L;
			return result;
		}
	}	// namespace anonymous


	glucose_unit get_default_glucose_display_unit( ) {
		return get_static_glucose_unit( );
	}

	void set_default_glucose_display_unit( glucose_unit unit ) {
		get_static_glucose_unit( ) = unit;
	}

	glucose_t::glucose_t( double d ) noexcept:
			value{ std::move( d ) } { }

	glucose_t::~glucose_t( ) { }

	glucose_t::operator double( ) noexcept {
		return value;
	}
	
	void swap( glucose_t & lhs, glucose_t & rhs ) noexcept {
		using std::swap;
		swap( lhs.value, rhs.value );
	}

	glucose_t mmol_L( double d ) noexcept {
		return glucose_t{ to_mg_dL( d ) };
	}

	glucose_t mg_dL( double d ) noexcept {
		return glucose_t{ d };
	}

	std::string glucose_t::to_string( ) const {
		std::stringstream ss;
		switch( get_static_glucose_unit( ) ) {
			case glucose_unit::mmol_L: ss << to_mmol_L( value ) << "mmol/L"; break;
			case glucose_unit::mg_dL: ss << "mg_dL"; break;
		}
		return ss.str( );
	}

	glucose_t & glucose_t::operator+=( glucose_t const & rhs ) noexcept {
		value += rhs.value;
		return *this;
	}

	glucose_t & glucose_t::operator-=( glucose_t const & rhs ) noexcept {
		value -= rhs.value;
		return *this;
	}

	glucose_t & glucose_t::scale( double factor ) noexcept {
		value *= factor;
		return *this;
	}

	glucose_t glucose_t::scale( double factor ) const noexcept {
		return glucose_t{ value * factor };
	}

	glucose_t operator+( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return glucose_t{ lhs.value + rhs.value };
	}

	glucose_t operator-( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return glucose_t{ lhs.value - rhs.value };
	}

	glucose_t operator-( glucose_t rhs ) noexcept {
		rhs.value *= -1.0; 
		return rhs;
	}

	bool operator==( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return lhs.value == rhs.value;
	}

	bool operator!=( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return lhs.value != rhs.value;
	}

	bool operator<( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return lhs.value < rhs.value;
	}

	bool operator>( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return lhs.value > rhs.value;
	}

	bool operator<=( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return lhs.value <= rhs.value;
	}

	bool operator>=( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return lhs.value >= rhs.value;
	}

	double glucose_t::as_mmol_L( ) const noexcept {
		return to_mmol_L( value );
	}

	double glucose_t::as_mg_dL( ) const noexcept {
		return value;
	}

	std::ostream & operator<<( std::ostream & os, glucose_t const & glucose ) {
		os << glucose.to_string( );
		return os;
	}
}  // namespace ns
 
ns::glucose_t operator"" _mmol_L( long double d ) noexcept {
	return ns::glucose_t{ ns::to_mg_dL( d ) };
}

ns::glucose_t operator"" _mg_dL( long double d ) noexcept {
	return ns::glucose_t{ static_cast<double>( d ) };
}

ns::glucose_t operator"" _mmol_L( unsigned long long i ) noexcept {
	return ns::glucose_t{ ns::to_mg_dL( i ) };
}

ns::glucose_t operator"" _mg_dL( unsigned long long i ) noexcept {
	return ns::glucose_t{ static_cast<double>( i ) };
}

