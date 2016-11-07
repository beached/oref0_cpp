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

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <date/date.h>

#include "iob_calc.h"

using namespace date;
using namespace std::chrono_literals;
namespace ns {
	namespace {
		size_t insulin_duration_to_idx( std::chrono::minutes const duration ) {	
			switch( duration.count( ) ) {
				case 180: return 0;
				case 210: return 1;
				case 240: return 2;
				case 300: return 3;
				case 360: return 4;
			}
			std::abort( );
		}

		constexpr auto insulin_duration_to_min( insulin_duration_t const duration ) {	
			switch( duration ) {
				case insulin_duration_t::t180: return 180min;
				case insulin_duration_t::t210: return 210min;
				case insulin_duration_t::t240: return 240min;
				case insulin_duration_t::t300: return 300min;
				case insulin_duration_t::t360: return 360min;
				default: return std::chrono::minutes{ std::numeric_limits<size_t>::max( ) };	// should never happen but may as well go for the stars
			}
		}


		template<size_t N>
			constexpr double to_power( double const & value ) {
				static_assert( N > 0, "" );
				double result = value;
				for( size_t n=1; n<N; ++n ) {
					result *= value;
				}
				return result;
			}
	}

	double insulin_on_board_pct( std::chrono::minutes const time_from_bolus_min, std::chrono::minutes const insulin_duration ) {
		double const params[5][5] = {
			{  99.951000000,  0.092550000, -0.01759000,  0.000135400, -0.00000032030 },	// t=180
			{  99.924242424,  0.282046657, -0.01489899,  0.000087168, -0.00000015900 },	// t=210 
			{  99.950000000, -0.090860000, -0.00551000,  0.000025300, -0.00000003310 },	// t=240
			{  99.300000000,  0.044900000, -0.00555000,  0.000023200, -0.00000002950 }, // t=300
			{  99.700000000,  0.063650000, -0.00409500,  0.000014130, -0.00000001493 }	// t=360
		};

		if( time_from_bolus_min <= 0min ) {
			return 1.0;
		} else if( time_from_bolus_min >= insulin_duration ) {
			return 0.0;
		} 
		double const t = time_from_bolus_min.count( );
		auto const & param = params[insulin_duration_to_idx( insulin_duration )];
		auto const p1 = param[1] * t;
		auto const p2 = param[2] * to_power<2>( t );
		auto const p3 = param[3] * to_power<3>( t );
		auto const p4 = param[4] * to_power<4>( t );

		auto percentage = (p4 + p3 + p2 + p1 + param[0])/100.0;

		// clamp value
		if( percentage > 1.0 ) {
			percentage = 1;
		} else if( percentage < 0.0 ) {
			percentage = 0;
		}
		return percentage;
	}

	insulin_dose::insulin_dose( insulin_t how_much, insulin_duration_t dia, timestamp_t when ):
			amount{ how_much },
			dose_time{ when },
			dose_dia{ insulin_duration_to_min( dia ) } { 

		assert( amount > 0.0_U );
	}

	std::ostream & operator<<( std::ostream & os, insulin_duration_t const & duration ) {
		os << insulin_duration_to_min( duration ).count( );
		return os;
	}

	std::istream & operator>>( std::istream & is, insulin_duration_t & duration ) {
		int_fast16_t tmp;
		is >> tmp;
		switch( tmp ) {
			case 180: duration = insulin_duration_t::t180; return is;
			case 210: duration = insulin_duration_t::t210; return is;
			case 240: duration = insulin_duration_t::t240; return is;
			case 300: duration = insulin_duration_t::t300; return is;
			case 360: duration = insulin_duration_t::t360; return is;
			default: throw std::runtime_error( "Unknown insulin_duration_t value" );
		}
	}

}	// namespace ns

