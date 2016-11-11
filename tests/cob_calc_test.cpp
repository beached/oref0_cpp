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

#define BOOST_TEST_MODULE iob_calc_test 
#include <boost/test/unit_test.hpp>

#include <iostream>

#include "diabetic_calcs.h"

BOOST_AUTO_TEST_CASE( glucose_unit_001 ) {
	using namespace ns;
	using namespace std::chrono_literals;
	using namespace std::chrono;

	auto const D = 100_g_CHO;
	auto const AT = 180min;

	carb_t last_value = 0_g_CHO;
	for( auto t = 0min; t <= AT; t += 5min ) {
		auto const result = calculations::calc_cob( D, AT, t ); 
		std::cout << "t=" << t.count( ) << " cob=" << result << " delta=" << (last_value - result) << '\n';
		last_value = result;
	}
}
