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

#include <sstream>
#include <string>
#include <chrono>

#include "data_types.h"
#include "glucose_unit.h"

namespace ns {
	struct glucose_rate_t{
		glucose_t value;

		explicit glucose_rate_t( glucose_t glucose ) noexcept;
		glucose_t glucose_per( ns::duration_minutes_t const & time_period ) const;
		~glucose_rate_t( );

		glucose_rate_t( ) = default;
		glucose_rate_t( glucose_rate_t const & ) = default;
		glucose_rate_t( glucose_rate_t && ) = default;
		glucose_rate_t & operator=( glucose_rate_t const & ) = default;
		glucose_rate_t & operator=( glucose_rate_t && ) = default;

		friend void swap( glucose_rate_t & lhs, glucose_rate_t & rhs ) noexcept;
		std::string to_string( ) const;
		glucose_rate_t & scale( real_t factor ) noexcept;
		glucose_rate_t scale( real_t factor ) const noexcept;

	};	// glucose_rate_t

	void swap( glucose_rate_t & lhs, glucose_rate_t & rhs ) noexcept;

	bool operator==( glucose_rate_t const & lhs, glucose_rate_t const & rhs ) noexcept;
	bool operator!=( glucose_rate_t const & lhs, glucose_rate_t const & rhs ) noexcept;
	bool operator<( glucose_rate_t const & lhs, glucose_rate_t const & rhs ) noexcept;
	bool operator>( glucose_rate_t const & lhs, glucose_rate_t const & rhs ) noexcept;
	bool operator<=( glucose_rate_t const & lhs, glucose_rate_t const & rhs ) noexcept;
	bool operator>=( glucose_rate_t const & lhs, glucose_rate_t const & rhs ) noexcept;

	template<typename... Args>
	glucose_rate_t operator/( glucose_t const & lhs, std::chrono::duration<Args...> const & rhs ) noexcept {
		real_t const factor = static_cast<real_t>(std::chrono::duration_cast<std::chrono::seconds>( rhs ).count( ))/3600.0;
		return glucose_rate_t{ lhs.scale( factor ) };
	}

	std::ostream & operator<<( std::ostream & os, glucose_rate_t const & glucose_rate );
}    // namespace ns

ns::glucose_rate_t operator"" _mg_dL_hr( long double d ) noexcept;
ns::glucose_rate_t operator"" _mg_dL_hr( unsigned long long i ) noexcept;
ns::glucose_rate_t operator"" _mmol_L_hr( long double d ) noexcept;
ns::glucose_rate_t operator"" _mmol_L_hr( unsigned long long i ) noexcept;

