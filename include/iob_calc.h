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
//
// Adapted from algorithm found at https://raw.githubusercontent.com/Perceptus/iob/master/insulin_on_board_pct.js


// insulin_duration in hours
// curves based on walsh iob curves
//

// 3 hour, 20 minute interval data points:
//  0   - 100%
//  20  - 95.797952%
//  40  - 83.354632%
//  60  - 67.27531199999999%
//  80  - 50.984312%
//  100 - 36.67599999999999%
//  120 - 25.314791999999983%
//  140 - 16.635152000000033%
//  160 - 9.141592000000088%
//  180 - 0%

// 3.5 hour, 20 minute interval data points:
//  0   - 100%
//  20  - 100%
//  40  - 92.539436704%
//  60  - 79.978325844%
//  80  - 65.25181498399999%
//  100 - 50.407008123999994%
//  120 - 36.88044926399999%
//  140 - 25.498122403999957%
//  160 - 16.475451543999995%
//  180 - 9.417300684000026%
//  200 - 3.317973824000049%
//  210 - 0%

// 4 hour, 20 minute interval data points:
//  0   - 100%
//  20  - 96.125904%
//  40  - 89.034064%
//  60  - 79.698224%
//  80  - 69.015024%
//  100 - 57.754000000000005%
//  120 - 46.55758399999999%
//  140 - 35.94110399999998%
//  160 - 26.292783999999997%
//  180 - 17.873744000000002%
//  200 - 10.81799999999997%
//  220 - 5.132463999999942%
//  240 - 0%

// 5 hour, 20 minute interval data points:
//  0   - 100%
//  20  - 98.15888%
//  40  - 93.62528%
//  60  - 86.64287999999999%
//  80  - 78.04208%
//  100 - 68.53999999999999%
//  120 - 58.74048%
//  140 - 49.13408%
//  160 - 40.098079999999996%
//  180 - 31.896479999999983%
//  200 - 24.680000000000035%
//  220 - 18.486080000000015%
//  240 - 13.238880000000023%
//  260 - 8.749280000000013%
//  280 - 4.714880000000036%
//  300 - 0%

// 6 hour, 20 minute interval data points:
//  0   - 100%
//  20  - 99.4456512%
//  40  - 96.5600992%
//  60  - 91.6355872%
//  80  - 85.2070272%
//  100 - 77.75200000000001%
//  120 - 69.69075520000001%
//  140 - 61.386211200000005%
//  160 - 53.14395520000001%
//  180 - 45.21224320000001%
//  200 - 37.78200000000004%
//  220 - 30.986819200000042%
//  240 - 24.902963200000016%
//  260 - 19.549363200000002%
//  280 - 14.887619200000003%
//  300 - 10.822000000000045%
//  320 - 7.199443200000033%
//  340 - 3.8095552000001334%
//  360 - 0%

#pragma once

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstdio>

#include <date/date.h>

namespace ns {
	enum class insulin_duration_t: int16_t { t180 = 180, t210 = 210, t240 = 240, t300 = 300, t360 = 360 };
	double insulin_on_board_pct( double const time_from_bolus_min, insulin_duration_t const insulin_duration );

	struct dose {
		using timestamp_t = std::chrono::system_clock::time_point;
		double amount;	// Amount of insulin
		timestamp_t dose_time;
		insulin_duration_t dose_dia;	// DIA in minutes, must be > 0
		
		dose( double how_much, insulin_duration_t dia = insulin_duration_t::t240, timestamp_t when = std::chrono::system_clock::now( ) );

		dose( ) = delete;
		~dose( ) = default;
		dose( dose const & ) = default;
		dose( dose && ) = default;
		dose & operator=( dose const & ) = default;
		dose & operator=( dose && ) = default;
	};

	std::ostream & operator<<( std::ostream & os, insulin_duration_t const & duration );
	std::istream & operator>>( std::istream & is, insulin_duration_t & duration );
}	// namespace ns

