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

#include <cmath>

namespace daw {
	class dbl_error {
		double m_value;
		double m_error;
	public:
		dbl_error( ) noexcept;
		~dbl_error( );
		explicit dbl_error( double value ) noexcept;
		dbl_error( double value, double error ) noexcept;

		dbl_error( dbl_error const & other ) = default;
		dbl_error( dbl_error && other ) = default;
		dbl_error & operator=( dbl_error const & rhs ) = default;
		dbl_error & operator=( dbl_error && rhs ) = default;

		double & operator*( ) noexcept;
		double const & operator*( ) const noexcept;

		double const & value( ) const noexcept;
		double & value( ) noexcept;
		double const & error( ) const noexcept;
		double & error( ) noexcept;

		friend dbl_error operator+( dbl_error lhs, dbl_error const & rhs ) noexcept;
		friend dbl_error operator-( dbl_error lhs, dbl_error const & rhs ) noexcept;
		friend dbl_error operator*( dbl_error const & lhs, dbl_error const & rhs ) noexcept;
		friend dbl_error operator/( dbl_error const & lhs, dbl_error const & rhs ) noexcept;

		friend dbl_error fabs( dbl_error value ) noexcept;
		friend dbl_error pow( dbl_error const & base, dbl_error const & exponent ) noexcept;
		friend dbl_error sqrt( dbl_error const & value ) noexcept;

		template<typename N> 
		friend dbl_error pow( dbl_error const & base, N const & exponent ) noexcept;
	};	// dbl_error

	dbl_error operator+( dbl_error lhs, dbl_error const & rhs ) noexcept;
	dbl_error operator-( dbl_error lhs, dbl_error const & rhs ) noexcept;
	dbl_error operator*( dbl_error const & lhs, dbl_error const & rhs ) noexcept;
	dbl_error operator/( dbl_error const & lhs, dbl_error const & rhs ) noexcept;

	dbl_error fabs( dbl_error value ) noexcept;
	dbl_error pow( dbl_error const & base, dbl_error const & exponent ) noexcept;
	dbl_error sqrt( dbl_error const & value ) noexcept;

	template<typename N>
	dbl_error pow( dbl_error const & base, N const & exponent ) noexcept {
		// TODO: determine what to do with a 0
		dbl_error result{ pow( base.m_value, exponent ) };
		result.m_error = FP_ZERO == std::fpclassify( base.m_value ) ? std::fabs( exponent ) * (base.m_error/std::fabs(base.m_value)) * result.m_value : base.m_error;
		return result;
	}
}    // namespace daw

