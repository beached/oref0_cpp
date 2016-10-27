//
// The MIT License (MIT)
//
// Copyright (c) 2015 Perceptus.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
//  .88888.  dP                            888888ba
// d8'   `88 88                            88    `8b
// 88        88 dP    dP .d8888b. .d8888b. 88     88 dP    dP 88d888b.
// 88   YP88 88 88    88 88'  `"" 88'  `88 88     88 88    88 88'  `88
// Y8.   .88 88 88.  .88 88.  ... 88.  .88 88    .8P 88.  .88 88    88
//  `88888'  dP `88888P' `88888P' `88888P' 8888888P  `8888P88 dP    dP
//                                                        .88
//                                                    d8888P
// dP                    888888ba                                                 dP
// 88                    88    `8b                                                88
// 88d888b. dP    dP    a88aaaa8P' .d8888b. 88d888b. .d8888b. .d8888b. 88d888b. d8888P dP    dP .d8888b.    .d8888b. 88d888b. .d8888b.
// 88'  `88 88    88     88        88ooood8 88'  `88 88'  `"" 88ooood8 88'  `88   88   88    88 Y8ooooo.    88'  `88 88'  `88 88'  `88
// 88.  .88 88.  .88     88        88.  ... 88       88.  ... 88.  ... 88.  .88   88   88.  .88       88 dP 88.  .88 88       88.  .88
// 88Y8888' `8888P88     dP        `88888P' dP       `88888P' `88888P' 88Y888P'   dP   `88888P' `88888P' 88 `88888P' dP       `8888P88
//               .88                                                   88                                                          .88
//           d8888P                                                    dP                                                      d8888P
//
// Education is crucial in the treatment of complex and dynamic diseases such as T1D. When patients are aware of the elements that affect
// variations in their disease, they are able to better understand their situation enabling them to optimize their treatments.
//
// Accessibility through the primary method of communication in the present: Digital communication. Live data should be accessible when
// the patient needs it in the easiest possible way.
//
// Autonomy is the dream of every patient with T1D. Life does not happen while being in front of a computer or staring at data on your
// phone. Most of the times where deviations happen, patients are busy living their lives.
//
// Gustavo Muñoz ( @bustavo )
// Kenneth Stack
//

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstdio>

#include <date/date.h>

#include "iob_calc.h"

namespace ns {
	namespace {
		constexpr size_t insulin_duration_to_idx( insulin_duration_t const duration ) {	
			switch( duration ) {
				case insulin_duration_t::t180: return 0;
				case insulin_duration_t::t210: return 1;
				case insulin_duration_t::t240: return 2;
				case insulin_duration_t::t300: return 3;
				case insulin_duration_t::t360: return 4;
			}
			throw std::runtime_error( "Unknown insulin_duration_t" );
		}

		template<size_t N, typename T>
		constexpr T to_power( T value ) {
			static_assert( N > 0, "" );
			for( size_t n=1; n<N; ++n ) {
				value *= value;
			}
			return value;
		}
	}
	double insulin_on_board_pct( double const time_from_bolus_min, insulin_duration_t insulin_duration ) {
		double const params[5][5] = {
			{  99.951000000,  0.092550000, -0.01759000,  0.000135400, -0.00000032030 },	// t=180
			{  99.924242424,  0.282046657, -0.01489899,  0.000087168, -0.00000015900 },	// t=210 
			{  99.950000000, -0.090860000, -0.00551000,  0.000025300, -0.00000003310 },	// t=240
			{  99.300000000,  0.044900000, -0.00555000,  0.000023200, -0.00000002950 }, // t=300
			{  99.700000000,  0.063650000, -0.00409500,  0.000014130, -0.00000001493 }	// t=360
		};

		if( time_from_bolus_min <= 0 ) {
			return 1.0;
		} else if( time_from_bolus_min >= static_cast<double>(insulin_duration) ) {
			return 0.0;
		} 

		auto const param = params[insulin_duration_to_idx( insulin_duration )];
		auto const p1 = param[1] * time_from_bolus_min;
		auto const p2 = param[2] * to_power<2>( time_from_bolus_min );
		auto const p3 = param[3] * to_power<3>( time_from_bolus_min );
		auto const p4 = param[4] * to_power<4>( time_from_bolus_min );

		auto percentage = (param[0] + p1 + p2 + p3 + p4)/100.0;

		// clamp value
		if( percentage > 1.0 ) {
			percentage = 1.0;
		} else if( percentage < 0 ) {
			percentage = 0;
		}

		return percentage;
	}

	dose::dose( double how_much, insulin_duration_t dia, timestamp_t when ):
		amount{ how_much },
		dose_time{ when },
		dose_dia{ dia } { 

			assert( amount > 0.0 );
		}
}	// namespace ns

