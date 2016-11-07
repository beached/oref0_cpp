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

#include <string>
#include "data_types.h"

namespace ns {
	struct insulin_t{
		real_t value;

		explicit insulin_t( real_t d ) noexcept;
		~insulin_t( );
		explicit operator real_t( ) noexcept;

		insulin_t( ) = default;
		insulin_t( insulin_t const & ) = default;
		insulin_t( insulin_t && ) = default;
		insulin_t & operator=( insulin_t const & ) = default;
		insulin_t & operator=( insulin_t && ) = default;
		friend void swap( insulin_t & lhs, insulin_t & rhs ) noexcept;
		std::string to_string( ) const;

		insulin_t & operator+=( insulin_t const & rhs ) noexcept;
		insulin_t & operator-=( insulin_t const & rhs ) noexcept;

		insulin_t & scale( real_t factor ) noexcept;
		insulin_t scale( real_t factor ) const noexcept;
	};	// insulin_t

	void swap( insulin_t & lhs, insulin_t & rhs ) noexcept;
	std::ostream & operator<<( std::ostream & os, insulin_t const & glucose );	

	insulin_t operator+( insulin_t lhs, insulin_t const & rhs ) noexcept;
	insulin_t operator-( insulin_t lhs, insulin_t const & rhs ) noexcept;
	insulin_t operator-( insulin_t rhs ) noexcept;
	bool operator==( insulin_t const & lhs, insulin_t const & rhs ) noexcept;
	bool operator!=( insulin_t const & lhs, insulin_t const & rhs ) noexcept;
	bool operator<( insulin_t const & lhs, insulin_t const & rhs ) noexcept;
	bool operator>( insulin_t const & lhs, insulin_t const & rhs ) noexcept;
	bool operator<=( insulin_t const & lhs, insulin_t const & rhs ) noexcept;
	bool operator>=( insulin_t const & lhs, insulin_t const & rhs ) noexcept;
}    // namespace ns

ns::insulin_t operator"" _U( long double d ) noexcept;
ns::insulin_t operator"" _U( unsigned long long i ) noexcept;
