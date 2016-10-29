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

#include "glucose_unit.h"

BOOST_AUTO_TEST_CASE( glucose_unit_001 ) {
	daw::glucose::mmol_L ca_01{ 5.5 };
	daw::glucose::mg_dL us_01{ ca_01 };
	daw::glucose::mg_dL us_02{ 100 };
	daw::glucose::mmol_L ca_02{ us_02 };
	daw::glucose::mmol_L ca_03{ -5.5 };
	
	std::cout << "E: 5.5mmol/L -> mg/dL\n";
	std::cout << "R: " << ca_01 << " " << us_01 << '\n';
	std::cout << "E: 100mg/dL -> mmol/L\n";
	std::cout << ca_02 << " " << us_02 << '\n';
	std::cout << "Literals 5.5mmol/L and 100mg/dL\n";
	std::cout << 5.5_mmol_L << " " << 100_mg_dL << '\n';
	std::cout << "E: -5.5mmol/L\n";
	std::cout << ca_03 << std::endl;

	std::cout << "E: 5.5mmol/L -> mg/dL -> mmol/L\n";
	ca_01 = us_01;
	std::cout << ca_01 << std::endl;

	std::cout << "E: 5.5mmol/L * 5\n"; 
	std::cout << 5.5_mmol_L * 5 << std::endl;
}
