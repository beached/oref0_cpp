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
#include "glucose_unit.h"
#include "insulin_unit.h"

namespace ns {
	struct isf_t {
		glucose_t value;

		explicit constexpr isf_t( glucose_t carb ) noexcept : value{std::move( carb )} {}

		std::string to_string( ) const;

		constexpr isf_t &scale( real_t factor ) noexcept {
			value.scale( factor );
			return *this;
		}

		constexpr isf_t scale( real_t factor ) const noexcept {
			isf_t result{*this};
			result.scale( factor );
			return result;
		}
	}; // isf_t

	constexpr void swap( isf_t &lhs, isf_t &rhs ) noexcept {
		auto tmp = lhs.value;
		lhs.value = rhs.value;
		rhs.value = tmp;
	}

	std::ostream &operator<<( std::ostream &os, isf_t const &isf );

	constexpr insulin_t operator/( glucose_t const &lhs, isf_t const &rhs ) noexcept {
		auto tmp = lhs.value / rhs.value.value;
		insulin_t result{std::move( tmp )};
		return result;
	}

	constexpr glucose_t operator*( insulin_t const &lhs, isf_t const &rhs ) noexcept {
		auto tmp = lhs.value * rhs.value.value;
		glucose_t result{std::move( tmp )};
		return result;
	}

	constexpr glucose_t operator*( isf_t const &lhs, insulin_t const &rhs ) noexcept {
		auto tmp = lhs.value.value * rhs.value;
		glucose_t result{std::move( tmp )};
		return result;
	}

	constexpr bool operator==( isf_t const &lhs, isf_t const &rhs ) noexcept {
		return lhs.value == rhs.value;
	}

	constexpr bool operator!=( isf_t const &lhs, isf_t const &rhs ) noexcept {
		return lhs.value != rhs.value;
	}

	constexpr bool operator<( isf_t const &lhs, isf_t const &rhs ) noexcept {
		return lhs.value < rhs.value;
	}

	constexpr bool operator>( isf_t const &lhs, isf_t const &rhs ) noexcept {
		return lhs.value > rhs.value;
	}

	constexpr bool operator<=( isf_t const &lhs, isf_t const &rhs ) noexcept {
		return lhs.value <= rhs.value;
	}

	constexpr bool operator>=( isf_t const &lhs, isf_t const &rhs ) noexcept {
		return lhs.value >= rhs.value;
	}
} // namespace ns

