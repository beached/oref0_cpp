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

#include <carb_time_unit.h>
#include <carb_unit.h>
#include <glucose_unit.h>
#include <icr_unit.h>
#include <insulin_time_unit.h>
#include <insulin_unit.h>
#include <isf_unit.h>

namespace ns {
	constexpr carb_rate_t operator*( insulin_rate_t lhs, icr_t const &rhs ) {
		using namespace std::chrono_literals;
		lhs.value.value *= rhs.value.value;
		return carb_rate_t{lhs.value.value, 60min};
	}

	constexpr carb_rate_t operator*( icr_t lhs, insulin_rate_t const & rhs ) {
		using namespace std::chrono_literals;
		lhs.value.value *= rhs.value.value;
		return carb_rate_t{lhs.value.value, 60min};
	}
}	// namespace ns


