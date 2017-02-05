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

#include "insulin_time_unit.h"

namespace ns {
	insulin_rate_t::insulin_rate_t( insulin_t insulin ) noexcept:
			value{ std::move( insulin ) } { }		

	insulin_rate_t::~insulin_rate_t( ) { }

	insulin_t insulin_rate_t::insulin_per( ns::duration_minutes_t const & time_period ) const {
		real_t const factor = static_cast<real_t>(time_period.count( ))/60.0;
		auto result = value.scale( factor );
		return result;
	}

	void swap( insulin_rate_t & lhs, insulin_rate_t & rhs ) noexcept {
		using std::swap;
		swap( lhs.value, rhs.value );
	}

	std::string insulin_rate_t::to_string( ) const {
		std::stringstream ss;
		ss << value << "/hr";
		return ss.str( );		
	}

	insulin_rate_t & insulin_rate_t::scale( real_t factor ) noexcept {
		value.scale( factor );
		return *this;
	}

	insulin_rate_t insulin_rate_t::scale( real_t factor ) const noexcept {
		insulin_rate_t result{ *this };
		result.scale( factor );
		return result;
	}

	bool operator==( insulin_rate_t const & lhs, insulin_rate_t const & rhs ) noexcept {
		return lhs.value == rhs.value;
	}

	bool operator!=( insulin_rate_t const & lhs, insulin_rate_t const & rhs ) noexcept {
		return lhs.value != rhs.value;
	}

	bool operator<( insulin_rate_t const & lhs, insulin_rate_t const & rhs ) noexcept {
		return lhs.value < rhs.value;
	}

	bool operator>( insulin_rate_t const & lhs, insulin_rate_t const & rhs ) noexcept {
		return lhs.value > rhs.value;
	}

	bool operator<=( insulin_rate_t const & lhs, insulin_rate_t const & rhs ) noexcept {
		return lhs.value <= rhs.value;
	}

	bool operator>=( insulin_rate_t const & lhs, insulin_rate_t const & rhs ) noexcept {
		return lhs.value >= rhs.value;
	}

	std::ostream & operator<<( std::ostream & os, insulin_rate_t const & insulin_rate ) {
		os << insulin_rate.to_string( );
		return os;
	}
}    // namespace ns




ns::insulin_rate_t operator"" _U_hr( long double d ) noexcept {
	return ns::insulin_rate_t{ ns::insulin_t{ static_cast<double>(d) } };
}
ns::insulin_rate_t operator"" _U_hr( unsigned long long i ) noexcept {
	return ns::insulin_rate_t{ ns::insulin_t{ static_cast<double>(i) } };
}
	
