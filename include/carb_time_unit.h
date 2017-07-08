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

#include <sstream>
#include <string>
#include <chrono>

#include "carb_unit.h"
#include "data_types.h"

namespace ns {
	namespace impl {
		constexpr carb_t scale_to_hour( carb_t carb, std::chrono::minutes const &duration ) {
			using namespace std::chrono_literals;
			if( duration != 60min ) {
				double const factor = 60.0 / static_cast<double>( duration.count( ) );
				carb.scale( factor );
			}
			return carb;
		}
	} // namespace impl

	class carb_rate_t {
		carb_t value;

	  public:
		constexpr carb_rate_t( carb_t carb, std::chrono::minutes const &duration ) noexcept
		    : value{impl::scale_to_hour( std::move( carb ), duration )} {}

		constexpr carb_rate_t( ) : carb_rate_t{0_g_CHO, std::chrono::minutes{60}} {}

		std::string to_string( ) const;

		constexpr carb_rate_t &scale( real_t const &factor ) noexcept {
			value.scale( factor );
			return *this;
		}

		constexpr carb_rate_t scale( real_t const &factor ) const noexcept {
			carb_rate_t result{*this};
			result.scale( factor );
			return result;
		}

		constexpr carb_t carbs_per( std::chrono::minutes const &time_period ) const {
			auto const factor = static_cast<real_t>( time_period.count( ) ) / 60.0;
			auto result = value.scale( factor );
			return result;
		}

		friend constexpr void swap( carb_rate_t &lhs, carb_rate_t &rhs ) noexcept {
			swap( lhs.value, rhs.value );
		}
		friend constexpr bool operator==( carb_rate_t const &lhs, carb_rate_t const &rhs ) noexcept {
			return lhs.value == rhs.value;
		}

		friend constexpr bool operator!=( carb_rate_t const &lhs, carb_rate_t const &rhs ) noexcept {
			return lhs.value != rhs.value;
		}

		friend constexpr bool operator<( carb_rate_t const &lhs, carb_rate_t const &rhs ) noexcept {
			return lhs.value < rhs.value;
		}

		friend constexpr bool operator>( carb_rate_t const &lhs, carb_rate_t const &rhs ) noexcept {
			return lhs.value > rhs.value;
		}

		friend constexpr bool operator<=( carb_rate_t const &lhs, carb_rate_t const &rhs ) noexcept {
			return lhs.value <= rhs.value;
		}

		friend constexpr bool operator>=( carb_rate_t const &lhs, carb_rate_t const &rhs ) noexcept {
			return lhs.value >= rhs.value;
		}

		friend constexpr std::chrono::minutes operator/( carb_t const &lhs, carb_rate_t const &rhs ) noexcept {
			return std::chrono::minutes{static_cast<intmax_t>( lhs.value * 60.0 / rhs.value.value )};
		}
	}; // carb_rate_t

	template<typename... Args>
	carb_rate_t operator/( carb_t const &lhs, std::chrono::duration<Args...> const &rhs ) noexcept {
		auto const tp = std::chrono::duration_cast<std::chrono::minutes>( rhs );
		return carb_rate_t{lhs, tp};
	}

	std::ostream &operator<<( std::ostream &os, carb_rate_t const &carb_rate );
} // namespace ns

constexpr ns::carb_rate_t operator"" _g_CHO_hr( long double d ) noexcept {
	using namespace std::chrono_literals;
	return ns::carb_rate_t{ ns::carb_t{ static_cast<double>(d) }, 60min };
}

constexpr ns::carb_rate_t operator"" _g_CHO_hr( unsigned long long i ) noexcept {
	using namespace std::chrono_literals;
	return ns::carb_rate_t{ ns::carb_t{ static_cast<double>(i) }, 60min };
}

constexpr ns::carb_rate_t operator"" _g_CHO_min( long double d ) noexcept {
	using namespace std::chrono_literals;
	return ns::carb_rate_t{ ns::carb_t{ static_cast<double>(d) }, 1min };
}

constexpr ns::carb_rate_t operator"" _g_CHO_min( unsigned long long i ) noexcept {
	using namespace std::chrono_literals;
	return ns::carb_rate_t{ ns::carb_t{ static_cast<double>(i) }, 1min };
}

