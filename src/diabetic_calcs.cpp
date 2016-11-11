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

#include <daw/daw_exception.h>

#include <date/date.h>
#include <chrono>

#include "units.h"

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace date;

namespace ns {
	namespace calculations {
		using namespace std::chrono_literals;

		ns::carb_rate_t	liver_glucose_output( ns::basal_rate_t const & b, ns::icr_t const & icr ) {
			return b * icr;
		}
		namespace {
			template<typename T>
			constexpr T sqr( T value ) {
				value *= value;
				return value;
			}
		}	// namespace anonymous

		carb_t calc_cob( carb_t const initial_dose, std::chrono::minutes const carb_activity_time, std::chrono::minutes const time_since_dose ) {
			daw::exception::daw_throw_on_true( time_since_dose < 0min );
			//std::cout << "calc_cob: initial_dose=" << initial_dose << " carb_activity_time=" << carb_activity_time.count( ) << "min time_since_dose=" << time_since_dose.count( ) << "min\n";
			// Used formula from https://github.com/Perceptus/GlucoDyn/blob/master/basic_math.pdf
			daw::exception::daw_throw_on_false( time_since_dose >= 0min );
			daw::exception::daw_throw_on_false( carb_activity_time >= 0min );
			auto result = initial_dose;
			auto const t = static_cast<real_t>(time_since_dose.count( ));
			auto const AT = static_cast<real_t>(carb_activity_time.count( ));
			
			if( time_since_dose < carb_activity_time/2.0 ) {
				real_t c{ initial_dose.value * 2.0 * sqr( static_cast<real_t>(time_since_dose.count( )))/sqr(static_cast<real_t>(carb_activity_time.count())) };
				result -= carb_t{ c };
				
			} else if( time_since_dose < carb_activity_time ) {
				auto const D = initial_dose.value;
				real_t c{  -D + ((4.0*D)/AT)*(t - (sqr(t)/(2.0*AT))) };
				result -= carb_t{ c };
			} else {
				result = 0_g_CHO;
			}
			return result; 
		}


	}	// namespace calculations
}    // namespace ns

