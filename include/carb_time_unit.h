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

#pragma once

#include <sstream>
#include <string>
#include <chrono>

#include "carb_unit.h"
#include "data_types.h"

namespace ns {
	class carb_rate_t{
		carb_t value;
	public:
		carb_rate_t( );
		carb_rate_t( carb_t carb, std::chrono::minutes const & duration ) noexcept;

		~carb_rate_t( ); 
		carb_rate_t( carb_rate_t const & ) = default;
		carb_rate_t( carb_rate_t && ) = default;
		carb_rate_t & operator=( carb_rate_t const & ) = default;
		carb_rate_t & operator=( carb_rate_t && ) = default;

		friend void swap( carb_rate_t & lhs, carb_rate_t & rhs ) noexcept;

		std::string to_string( ) const;

		carb_rate_t & scale( real_t const & factor ) noexcept;
		carb_rate_t scale( real_t const & factor ) const noexcept;
		
		carb_t carbs_per( std::chrono::minutes const & time_period ) const;

		friend void swap( carb_rate_t & lhs, carb_rate_t & rhs ) noexcept;
		friend bool operator==( carb_rate_t const & lhs, carb_rate_t const & rhs ) noexcept;
		friend bool operator!=( carb_rate_t const & lhs, carb_rate_t const & rhs ) noexcept;
		friend bool operator<( carb_rate_t const & lhs, carb_rate_t const & rhs ) noexcept;
		friend bool operator>( carb_rate_t const & lhs, carb_rate_t const & rhs ) noexcept;
		friend bool operator<=( carb_rate_t const & lhs, carb_rate_t const & rhs ) noexcept;
		friend bool operator>=( carb_rate_t const & lhs, carb_rate_t const & rhs ) noexcept;
		friend std::chrono::minutes operator/( carb_t const & lhs, carb_rate_t const & rhs ) noexcept;
	};	// carb_rate_t

	void swap( carb_rate_t & lhs, carb_rate_t & rhs ) noexcept;
	bool operator==( carb_rate_t const & lhs, carb_rate_t const & rhs ) noexcept;
	bool operator!=( carb_rate_t const & lhs, carb_rate_t const & rhs ) noexcept;
	bool operator<( carb_rate_t const & lhs, carb_rate_t const & rhs ) noexcept;
	bool operator>( carb_rate_t const & lhs, carb_rate_t const & rhs ) noexcept;
	bool operator<=( carb_rate_t const & lhs, carb_rate_t const & rhs ) noexcept;
	bool operator>=( carb_rate_t const & lhs, carb_rate_t const & rhs ) noexcept;
		
	std::chrono::minutes operator/( carb_t const & lhs, carb_rate_t const & rhs ) noexcept;

	template<typename... Args>
	carb_rate_t operator/( carb_t const & lhs, std::chrono::duration<Args...> const & rhs ) noexcept {
		auto const tp = std::chrono::duration_cast<std::chrono::minutes>( rhs );
		return carb_rate_t{ lhs, tp };
	}

	std::ostream & operator<<( std::ostream & os, carb_rate_t const & carb_rate );
}    // namespace ns

ns::carb_rate_t operator"" _g_CHO_hr( long double d ) noexcept;
ns::carb_rate_t operator"" _g_CHO_hr( unsigned long long i ) noexcept;
ns::carb_rate_t operator"" _g_CHO_min( long double d ) noexcept;
ns::carb_rate_t operator"" _g_CHO_min( unsigned long long i ) noexcept;
