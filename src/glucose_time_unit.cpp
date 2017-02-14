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

#include "glucose_unit.h"
#include "glucose_time_unit.h"

namespace ns {
	glucose_rate_t::glucose_rate_t( glucose_t glucose ) noexcept:
			value{ std::move( glucose ) } { }		

	glucose_rate_t::~glucose_rate_t( ) { }

	glucose_t glucose_rate_t::glucose_per( ns::duration_minutes_t const & time_period ) const {
		real_t const factor = static_cast<real_t>(time_period.count( ))/60.0;
		auto result = value.scale( factor );
		return result;
	}

	void swap( glucose_rate_t & lhs, glucose_rate_t & rhs ) noexcept {
		using std::swap;
		swap( lhs.value, rhs.value );
	}

	std::string glucose_rate_t::to_string( ) const {
		std::stringstream ss;
		ss << value << "/hr";
		return ss.str( );		
	}

	glucose_rate_t & glucose_rate_t::scale( real_t factor ) noexcept {
		value.scale( factor );
		return *this;
	}

	glucose_rate_t glucose_rate_t::scale( real_t factor ) const noexcept {
		glucose_rate_t result{ *this };
		result.scale( factor );
		return result;
	}

	bool operator==( glucose_rate_t const & lhs, glucose_rate_t const & rhs ) noexcept {
		return lhs.value == rhs.value;
	}

	bool operator!=( glucose_rate_t const & lhs, glucose_rate_t const & rhs ) noexcept {
		return lhs.value != rhs.value;
	}

	bool operator<( glucose_rate_t const & lhs, glucose_rate_t const & rhs ) noexcept {
		return lhs.value < rhs.value;
	}

	bool operator>( glucose_rate_t const & lhs, glucose_rate_t const & rhs ) noexcept {
		return lhs.value > rhs.value;
	}

	bool operator<=( glucose_rate_t const & lhs, glucose_rate_t const & rhs ) noexcept {
		return lhs.value <= rhs.value;
	}

	bool operator>=( glucose_rate_t const & lhs, glucose_rate_t const & rhs ) noexcept {
		return lhs.value >= rhs.value;
	}

	std::ostream & operator<<( std::ostream & os, glucose_rate_t const & glucose_rate ) {
		os << glucose_rate.to_string( );
		return os;
	}
}    // namespace ns




ns::glucose_rate_t operator"" _mg_dL_hr( long double d ) noexcept {
	return ns::glucose_rate_t{ ns::glucose_t{ static_cast<double>(d) } };
}
ns::glucose_rate_t operator"" _mg_dL_hr( unsigned long long i ) noexcept {
	return ns::glucose_rate_t{ ns::glucose_t{ static_cast<double>(i) } };
}
	
ns::glucose_rate_t operator"" _mmol_L_hr( long double d ) noexcept {
	return ns::glucose_rate_t{ ns::glucose_t{ ns::to_mg_dL( d ) } };
}

ns::glucose_rate_t operator"" _mmol_L_hr( unsigned long long i ) noexcept {
	return ns::glucose_rate_t{ ns::glucose_t{ ns::to_mg_dL( i ) } };
}

