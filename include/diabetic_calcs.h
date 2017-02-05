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

#include <algorithm>
#include <numeric>

#include "diabetic_calcs.h"
#include "units.h"

namespace ns {
	namespace calculations {
		carb_rate_t	liver_glucose_output( insulin_rate_t const & b, icr_t const & icr );
		carb_t calc_cob( carb_t const initial_dose, ns::duration_minutes_t const & carb_activity_time, ns::duration_minutes_t const & time_since_dose );

		template<typename Collection>
		auto linear_regression( Collection const & points ) {
			using value_t = std::decay_t<decltype( *std::begin( points ) )>;
			struct {
				value_t x = 0.0;
				value_t y = 0.0;
				value_t xy = 0.0;
				value_t x2 = 0.0;
			} init;
			auto const sum = std::accumulate( std::begin( points ), std::end( points ), init, []( auto last, auto const & point ) {
				last.x += point.x;
				last.y += point.y;
				last.xy += point.x * point.y;
				last.x2 += point.x * point.x;
				return last;
			} );

			struct {
				value_t slope = 0.0;
				value_t intercept = 0.0;
			} result;
			auto const count = static_cast<value_t>(std::distance( std::begin( points ), std::end( points ) ));
			result.slope = ((count * sum.xy) - (sum.x * sum.y))/((count * sum.x2) - (sum.x*sum.x));
			result.intercept = (sum.y * sum.x2 - (sum.x*sum.xy))/(count*sum.x2 - (sum.x*sum.x));
			return result;
		}

	}	// namespace calculations
}    // namespace ns

