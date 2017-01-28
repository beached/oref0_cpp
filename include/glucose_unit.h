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

#include <string>
#include "data_types.h"

namespace ns {
	enum class glucose_unit { mmol_L, mg_dL };

	glucose_unit get_default_glucose_display_unit( );
	void set_default_glucose_display_unit( glucose_unit unit );

	struct glucose_t {
		real_t value;

		explicit glucose_t( real_t d ) noexcept;
		~glucose_t( );

		operator real_t( ) noexcept;

		glucose_t( ) = default;
		glucose_t( glucose_t const & ) = default;
		glucose_t( glucose_t && ) = default;
		glucose_t & operator=( glucose_t const & ) = default;
		glucose_t & operator=( glucose_t && ) = default;
		friend void swap( glucose_t & lhs, glucose_t & rhs ) noexcept;

		std::string to_string( ) const;
		std::string to_string( glucose_unit unit ) const;

		glucose_t & operator+=( glucose_t const & rhs ) noexcept;
		glucose_t & operator-=( glucose_t const & rhs ) noexcept;
		glucose_t & operator-( ) noexcept;
		glucose_t operator-( ) const noexcept;

		glucose_t & scale( real_t factor ) noexcept;
		glucose_t scale( real_t factor ) const noexcept;

		real_t as_mmol_L( ) const noexcept;
		real_t as_mg_dL( ) const noexcept;
	};	// glucose_t

	void swap( glucose_t & lhs, glucose_t & rhs ) noexcept;

	glucose_t mmol_L( real_t d ) noexcept;
	glucose_t mg_dL( real_t d ) noexcept;

	glucose_t operator+( glucose_t lhs, glucose_t const & rhs ) noexcept;
	glucose_t operator-( glucose_t lhs, glucose_t const & rhs ) noexcept;
	bool operator==( glucose_t const & lhs, glucose_t const & rhs ) noexcept;
	bool operator!=( glucose_t const & lhs, glucose_t const & rhs ) noexcept;
	bool operator<( glucose_t const & lhs, glucose_t const & rhs ) noexcept;
	bool operator>( glucose_t const & lhs, glucose_t const & rhs ) noexcept;
	bool operator<=( glucose_t const & lhs, glucose_t const & rhs ) noexcept;
	bool operator>=( glucose_t const & lhs, glucose_t const & rhs ) noexcept;

	std::ostream & operator<<( std::ostream & os, glucose_t const & glucose );	
}	// namespace ns

ns::glucose_t operator"" _mmol_L( long double d ) noexcept;
ns::glucose_t operator"" _mg_dL( long double d ) noexcept;
ns::glucose_t operator"" _mmol_L( unsigned long long i ) noexcept;
ns::glucose_t operator"" _mg_dL( unsigned long long i ) noexcept;

