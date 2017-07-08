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

#pragma once

#include <string>

#include "data_types.h"

namespace ns {
	struct carb_t{
		real_t value;

		constexpr carb_t( real_t d ) noexcept : value{std::move( d )} {}

		std::string to_string( ) const;

		constexpr carb_t &operator+=( carb_t const &rhs ) noexcept {
			value += rhs.value;
			return *this;
		}

		constexpr carb_t &operator-=( carb_t const &rhs ) noexcept {
			value -= rhs.value;
			return *this;
		}

		constexpr carb_t &scale( real_t factor ) noexcept {
			value *= factor;
			return *this;
		}

		constexpr carb_t scale( real_t factor ) const noexcept {
			carb_t result{*this};
			result.value *= factor;
			return result;
		}

	};	// carb_t

	std::string to_string( carb_t const & c );

	constexpr void swap( carb_t & lhs, carb_t & rhs ) noexcept {
		auto tmp = lhs.value;
		lhs.value = rhs.value;
		rhs.value = tmp;
	}

	std::ostream & operator<<( std::ostream & os, carb_t const & glucose );	

	constexpr carb_t operator+( carb_t lhs, carb_t const & rhs ) noexcept {
		lhs.value += rhs.value;
		return lhs;
	}

	constexpr carb_t operator-( carb_t lhs, carb_t const & rhs ) noexcept {
		lhs.value -= rhs.value;
		return lhs;
	}

	constexpr carb_t operator-( carb_t rhs ) noexcept {
		rhs.value *= -1.0; 
		return rhs;
	}

	constexpr bool operator==( carb_t const & lhs, carb_t const & rhs ) noexcept {
		return lhs.value == rhs.value;
	}

	constexpr bool operator!=( carb_t const & lhs, carb_t const & rhs ) noexcept {
		return lhs.value != rhs.value;
	}

	constexpr bool operator<( carb_t const & lhs, carb_t const & rhs ) noexcept {
		return lhs.value < rhs.value;
	}

	constexpr bool operator>( carb_t const & lhs, carb_t const & rhs ) noexcept {
		return lhs.value > rhs.value;
	}

	constexpr bool operator<=( carb_t const & lhs, carb_t const & rhs ) noexcept {
		return lhs.value <= rhs.value;
	}

	constexpr bool operator>=( carb_t const & lhs, carb_t const & rhs ) noexcept {
		return lhs.value >= rhs.value;
	}
}    // namespace ns

constexpr ns::carb_t operator"" _g_CHO( long double d ) noexcept {
	return ns::carb_t{ static_cast<double>( d ) };
}

constexpr ns::carb_t operator"" _g_CHO( unsigned long long i ) noexcept {
	return ns::carb_t{ static_cast<double>( i ) };
}

