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

#define BOOST_TEST_MODULE iob_calc_test 
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <random>

#include <daw/json/daw_json_link.h>

#include "iob_calc.h"

struct test_data_item_t: public daw::json::JsonLink<test_data_item_t> {
	int16_t insulin_duration;
	int16_t time_offset;
	double expected;

	test_data_item_t( test_data_item_t && ) = default;
	test_data_item_t & operator=( test_data_item_t && ) = default;
	
	test_data_item_t( int16_t duration, int16_t timeOffset, double expectedResult ):
			daw::json::JsonLink<test_data_item_t>{ },
			insulin_duration{ duration },
			time_offset{ timeOffset },
			expected{ expectedResult } {
			
		link_integral( "insulin_duration", insulin_duration );
		link_integral( "time_offset", time_offset );
		link_real( "expected", expected );
	}

	test_data_item_t( ):
			test_data_item_t{ 180, 0, 1.0 } { }

	test_data_item_t( test_data_item_t const & other ):
			test_data_item_t{ other.insulin_duration, other.time_offset, other.expected } { }

	test_data_item_t & operator=( test_data_item_t const & rhs ) {
		if( this != &rhs ) {
			test_data_item_t tmp{ rhs };
			using std::swap;
			swap( *this, tmp );
		}
		return *this;
	}

	~test_data_item_t( );
};	// test_data_item_t

test_data_item_t::~test_data_item_t( ) { }

struct test_data_t: public daw::json::JsonLink<test_data_t> {
	std::vector<test_data_item_t> tests;

	test_data_t( test_data_t && other ) = default;
	test_data_t & operator=( test_data_t && ) = default;

	test_data_t( std::vector<test_data_item_t> Tests ):
			daw::json::JsonLink<test_data_t>{ },
			tests{ std::move( Tests ) } {
		
		link_array( "tests", tests );
	}

	test_data_t( ):
			test_data_t{ std::vector<test_data_item_t>{ } } { }

	~test_data_t( );

	test_data_t( test_data_t const & other ):
			test_data_t{ other.tests } { }

	test_data_t & operator=( test_data_t const & rhs ) {
		if( this != &rhs ) {
			using std::swap;
			test_data_t tmp{ rhs };
			swap( *this, tmp );
		}
		return *this;
	}
};	// test_data_t

test_data_t::~test_data_t( ) { }

BOOST_AUTO_TEST_CASE( insulin_on_board_pct, *boost::unit_test::tolerance( 1.0e-10 ) ) {
	auto test_data = daw::json::from_file<test_data_t>( "../tests/iob_calc_test.json" );
	for( auto const & test: test_data.tests ) {
		auto const result = ns::insulin_on_board_pct( std::chrono::minutes{ test.time_offset }, std::chrono::minutes{ test.insulin_duration } );
		std::cout << "insulin_action=" << test.insulin_duration << "min t=" << test.time_offset << "min" << std::setprecision( std::numeric_limits<double>::digits10 ) << " expected=" << test.expected << " result=" << result << " difference=" << (test.expected-result) << '\n';
		BOOST_TEST( test.expected == result );
	}

}
