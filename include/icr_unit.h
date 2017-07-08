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

#include "carb_unit.h"
#include "data_types.h"
#include "insulin_unit.h"

namespace ns {
	struct icr_t {
		carb_t value;

		explicit constexpr icr_t( carb_t carb ) noexcept : value{std::move( carb )} {}

		std::string to_string( ) const;

		constexpr icr_t &scale( real_t factor ) noexcept {
			value.scale( factor );
			return *this;
		}

		constexpr icr_t scale( real_t factor ) const noexcept {
			icr_t result{*this};
			result.scale( factor );
			return result;
		}
	}; // icr_t

	constexpr void swap( icr_t &lhs, icr_t &rhs ) noexcept {
		using std::swap;
		swap( lhs.value, rhs.value );
	}

	std::ostream &operator<<( std::ostream &os, icr_t const &icr );

	constexpr insulin_t operator/( carb_t const &lhs, icr_t const &rhs ) noexcept {
		auto tmp = lhs.value / rhs.value.value;
		insulin_t result{std::move( tmp )};
		return result;
	}

	constexpr carb_t operator*( icr_t const &lhs, insulin_t const &rhs ) noexcept {
		auto tmp = lhs.value.value * rhs.value;
		carb_t result{std::move( tmp )};
		return result;
	}

	constexpr carb_t operator*( insulin_t const &lhs, icr_t const &rhs ) noexcept {
		auto tmp = rhs.value.value * lhs.value;
		carb_t result{std::move( tmp )};
		return result;
	}

	constexpr bool operator==( icr_t const &lhs, icr_t const &rhs ) noexcept {
		return lhs.value == rhs.value;
	}

	constexpr bool operator!=( icr_t const &lhs, icr_t const &rhs ) noexcept {
		return lhs.value != rhs.value;
	}

	constexpr bool operator<( icr_t const &lhs, icr_t const &rhs ) noexcept {
		return lhs.value < rhs.value;
	}

	constexpr bool operator>( icr_t const &lhs, icr_t const &rhs ) noexcept {
		return lhs.value > rhs.value;
	}

	constexpr bool operator<=( icr_t const &lhs, icr_t const &rhs ) noexcept {
		return lhs.value <= rhs.value;
	}

	constexpr bool operator>=( icr_t const &lhs, icr_t const &rhs ) noexcept {
		return lhs.value >= rhs.value;
	}
} // namespace ns

