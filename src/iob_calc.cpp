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
	double insulin_on_board_pct( int16_t const time_from_bolus_min, insulin_duration_t insulin_duration ) {

		if( time_from_bolus_min <= 0 ) {
			return 1.0;
		} else if( time_from_bolus_min >= static_cast<int16_t>(insulin_duration) ) {
			return 0.0;
		} 

		auto const dbl_time_from_bolus_min = static_cast<double>(time_from_bolus_min);
		double percentage = 100.0;
		switch( insulin_duration ) {
			case insulin_duration_t::t180:
				percentage = -3.203e-7 * pow(dbl_time_from_bolus_min,4.0) + 1.354e-4 * pow(dbl_time_from_bolus_min,3.0) - 1.759e-2 * pow(dbl_time_from_bolus_min,2.0) + 9.255e-2 * dbl_time_from_bolus_min + 99.951;
				break;
			case insulin_duration_t::t210:
				percentage = -0.000000159 * pow(dbl_time_from_bolus_min,4.0) + 0.000087168 * pow(time_from_bolus_min,3.0) - 0.014898990 * pow(time_from_bolus_min,2.0) + 0.282046657 * time_from_bolus_min + 99.924242424;
				break;
			case insulin_duration_t::t240:
				percentage = -3.31e-8 * pow(dbl_time_from_bolus_min,4.0) + 2.53e-5 * pow(time_from_bolus_min,3.0) - 5.51e-3 * pow(time_from_bolus_min,2.0) - 9.086e-2 * time_from_bolus_min + 99.95;
				break;
			case insulin_duration_t::t300:
				percentage = -2.95e-8 * pow(dbl_time_from_bolus_min,4.0) + 2.32e-5 * pow(time_from_bolus_min,3.0) - 5.55e-3 * pow(time_from_bolus_min,2.0) + 4.49e-2 * time_from_bolus_min + 99.3;
				break;
			case insulin_duration_t::t360:
				percentage = -1.493e-8 * pow(dbl_time_from_bolus_min,4.0) + 1.413e-5 * pow(time_from_bolus_min,3.0) - 4.095e-3 * pow(time_from_bolus_min,2.0) + 6.365e-2 * time_from_bolus_min + 99.7;
				break;
		}
		percentage /= 100.0;

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

