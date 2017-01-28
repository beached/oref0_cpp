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
#include "carb_unit.h"
#include "insulin_unit.h"

namespace ns {
	struct icr_t{
		carb_t value;

		explicit icr_t( carb_t carb ) noexcept;
		~icr_t( );

		icr_t( ) = default;
		icr_t( icr_t const & ) = default;
		icr_t( icr_t && ) = default;
		icr_t & operator=( icr_t const & ) = default;
		icr_t & operator=( icr_t && ) = default;
		friend void swap( icr_t & lhs, icr_t & rhs ) noexcept;
		std::string to_string( ) const;

		icr_t & scale( real_t factor ) noexcept;
		icr_t scale( real_t factor ) const noexcept;
	};	// icr_t

	void swap( icr_t & lhs, icr_t & rhs ) noexcept;
	std::ostream & operator<<( std::ostream & os, icr_t const & icr );	

	insulin_t operator/( carb_t const & lhs, icr_t const & rhs ) noexcept;
	carb_t operator*( icr_t const & lhs, insulin_t const & rhs ) noexcept;
	carb_t operator*( insulin_t const & lhs, icr_t const & rhs ) noexcept;

	bool operator==( icr_t const & lhs, icr_t const & rhs ) noexcept;
	bool operator!=( icr_t const & lhs, icr_t const & rhs ) noexcept;
	bool operator<( icr_t const & lhs, icr_t const & rhs ) noexcept;
	bool operator>( icr_t const & lhs, icr_t const & rhs ) noexcept;
	bool operator<=( icr_t const & lhs, icr_t const & rhs ) noexcept;
	bool operator>=( icr_t const & lhs, icr_t const & rhs ) noexcept;
}    // namespace ns

