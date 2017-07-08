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
//
// Adapted from algorithm found at https://raw.githubusercontent.com/Perceptus/iob/master/insulin_on_board_pct.js

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <date/date.h>

#include "data_types.h"
#include "iob_calc.h"

using namespace date;
using namespace ns::chrono_literals;
namespace ns {
	namespace {
		std::ostream &operator<<( std::ostream &os, insulin_duration_t const &duration ) {
			os << insulin_duration_to_min( duration ).count( );
			return os;
		}

		std::istream &operator>>( std::istream &is, insulin_duration_t &duration ) {
			int_fast16_t tmp;
			is >> tmp;
			switch( tmp ) {
			case 180:
				duration = insulin_duration_t::t180;
				return is;
			case 210:
				duration = insulin_duration_t::t210;
				return is;
			case 240:
				duration = insulin_duration_t::t240;
				return is;
			case 300:
				duration = insulin_duration_t::t300;
				return is;
			case 360:
				duration = insulin_duration_t::t360;
				return is;
			default:
				throw std::runtime_error( "Unknown insulin_duration_t value" );
			}
		}
	} // namespace
} // namespace ns

