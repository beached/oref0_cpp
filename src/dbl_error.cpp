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

#include <cmath>

#include "dbl_error.h"

namespace daw {
		constexpr dbl_error::dbl_error( double value, double error ) noexcept:
				m_value{ value },
				m_error{ error } { }

		constexpr dbl_error::dbl_error( ) noexcept:
				dbl_error{ 0.0, 0.0 } { }

		constexpr dbl_error::dbl_error( double value ) noexcept:
				dbl_error{ value, 0.0 } { }

		dbl_error::~dbl_error( ) { }

		constexpr double & dbl_error::operator*( ) noexcept {
			return m_value;
		}

		constexpr double const & dbl_error::operator*( ) const noexcept {
			return m_value;
		}

		constexpr double const & dbl_error::value( ) const noexcept {
			return m_value;
		}

		constexpr double & dbl_error::value( ) noexcept {
			return m_value;
		}

		constexpr double const & dbl_error::error( ) const noexcept {
			return m_error;
		}

		constexpr double & dbl_error::error( ) noexcept {
			return m_error;
		}
		namespace {
			constexpr double pow2( double const & value ) noexcept {
				return value*value;
			}

			double sqrt_sum_of_sqrs( double const & v1, double const & v2 ) noexcept {
				return sqrt( pow2( v1 ) + pow2( v2 ) );
			}
		}	// namespace anonymous

		dbl_error operator+( dbl_error lhs, dbl_error const & rhs ) noexcept {
			lhs.m_value += rhs.m_value;
			lhs.m_error = sqrt_sum_of_sqrs( lhs.m_error, rhs.m_error );
			return lhs;
		}

		dbl_error operator-( dbl_error lhs, dbl_error const & rhs ) noexcept {
			lhs.m_value -= rhs.m_value;
			lhs.m_error = sqrt_sum_of_sqrs( lhs.m_error, rhs.m_error );
			return lhs;
		}

		dbl_error operator*( dbl_error const & lhs, dbl_error const & rhs ) noexcept {
			dbl_error result{ lhs.m_value * rhs.m_value }; 
			result.m_error = fabs( result.m_value ) * sqrt_sum_of_sqrs( lhs.m_error/lhs.m_value, rhs.m_error/rhs.m_value );
			return result;
		}

		dbl_error operator/( dbl_error const & lhs, dbl_error const & rhs ) noexcept {
			dbl_error result{ lhs.m_value / rhs.m_value }; 
			result.m_error = fabs( result.m_value ) * sqrt_sum_of_sqrs( lhs.m_error/lhs.m_value, rhs.m_error/rhs.m_value );
			return result;
		}

		dbl_error fabs( dbl_error value ) noexcept {
			value.m_value = fabs( value.m_value );
			return value;
		}

		dbl_error pow( dbl_error const & base, dbl_error const & exponent ) noexcept {

		}

		dbl_error sqrt( dbl_error const & value ) noexcept {
			dbl_error result{ sqrt( value.m_value ) };
			result.m_error = 

			return result;
		}


}    // namespace daw

