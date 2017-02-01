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

#include <array>
#include <boost/optional.hpp>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <unordered_map>
#include <string>
#include <boost/multiprecision/cpp_dec_float.hpp>

#include <daw/json/value_t.h>

namespace ns {
//	using real_t = boost::multiprecision::cpp_dec_float_50;		// ~1000 runs/second
//	using real_t = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<10>>; // ~2000 runs/second
	using real_t = double;	// 10000 runs/second
	using timestamp_t = std::chrono::time_point<std::chrono::system_clock>;
	using duration_t = std::chrono::milliseconds;
	using duration_hours_t = std::chrono::duration<double, std::ratio<3600l>>;
	using duration_minutes_t = std::chrono::duration<double, std::ratio<60l>>;

	struct glucose_status_t {
		real_t glucose;
		real_t delta;
		real_t avg_delta;
	};	// glucose_status_t

	struct iob_data_t {
		double bolussnooze;
		real_t activity;
		real_t iob;
		boost::optional<real_t> basaliob;
		boost::optional<real_t> netbasalinsulin;
		boost::optional<real_t> hightempinsulin;

		iob_data_t( double bolusSnooze, 
					real_t Activity, 
					real_t Iob, 
					boost::optional<real_t> basalIob, 
					boost::optional<real_t> netBasalInsulin, 
					boost::optional<real_t> highTempInsulin );


		iob_data_t( );
		~iob_data_t( );
		
		iob_data_t( iob_data_t const & ) = default;
		iob_data_t( iob_data_t && ) = default;
		iob_data_t & operator=( iob_data_t const & ) = default;
		iob_data_t & operator=( iob_data_t && ) = default;
	};	// iob_data_t

	struct autosense_data_t {
		double ratio;	
	};	// autosense_data_t

	struct meal_data_t {

	};	// meal_data_t

	struct current_temp_t {
		real_t rate;
	};	// current_temp_t

	struct determine_basal_exception: public std::runtime_error {
		template<typename... Args>
		determine_basal_exception( Args&&... args ):
				std::runtime_error( std::forward<Args>( args )... ) { }
	};	// determine_basal_exception

	namespace chrono_literals {
		duration_hours_t operator"" _hours( long double d ) noexcept;
		duration_hours_t operator"" _hours( unsigned long long i ) noexcept;
		duration_minutes_t operator"" _mins( long double d ) noexcept;
		duration_minutes_t operator"" _mins( unsigned long long i ) noexcept;
	}	// namespace chrono_literals
}    // namespace ns 

