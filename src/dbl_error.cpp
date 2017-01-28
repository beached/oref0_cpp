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

#include <cmath>
#include <exception>
#include <stdexcept>

#include "dbl_error.h"

namespace daw {
		dbl_error::dbl_error( ns::real_t value, ns::real_t error ) noexcept:
				m_value{ value },
				m_error{ error } { }

		dbl_error::dbl_error( ) noexcept:
				dbl_error{ 0.0, 0.0 } { }

		dbl_error::dbl_error( ns::real_t value ) noexcept:
				dbl_error{ value, 0.0 } { }

		dbl_error::~dbl_error( ) { }

		ns::real_t & dbl_error::operator*( ) noexcept {
			return m_value;
		}

		ns::real_t const & dbl_error::operator*( ) const noexcept {
			return m_value;
		}

		ns::real_t const & dbl_error::value( ) const noexcept {
			return m_value;
		}

		ns::real_t & dbl_error::value( ) noexcept {
			return m_value;
		}

		ns::real_t const & dbl_error::error( ) const noexcept {
			return m_error;
		}

		ns::real_t & dbl_error::error( ) noexcept {
			return m_error;
		}
		namespace {
			ns::real_t pow2( ns::real_t const & value ) noexcept {
				return value*value;
			}

			ns::real_t sqrt_sum_of_sqrs( ns::real_t const & v1, ns::real_t const & v2 ) noexcept {
				using std::sqrt;
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
			dbl_error result;
			result.m_value = lhs.m_value * rhs.m_value;
			result.m_error = abs( result.m_value ) * sqrt_sum_of_sqrs( lhs.m_error/lhs.m_value, rhs.m_error/rhs.m_value );
			return result;
		}

		dbl_error operator/( dbl_error const & lhs, dbl_error const & rhs ) noexcept {
			dbl_error result;
			result.m_value = lhs.m_value / rhs.m_value;
			result.m_error = abs( result.m_value ) * sqrt_sum_of_sqrs( lhs.m_error/lhs.m_value, rhs.m_error/rhs.m_value );
			return result;
		}

		dbl_error fabs( dbl_error value ) noexcept {
			value.m_value = abs( value.m_value );
			return value;
		}

		dbl_error pow( dbl_error const & base, dbl_error const & exponent ) noexcept {
			throw std::runtime_error{ "Not Implemented" };
			//return dbl_error{ };
		}

		dbl_error sqrt( dbl_error const & value ) noexcept {
			dbl_error result;
			using std::sqrt;
			result.m_value = sqrt( value.m_value );

			return result;
		}


}    // namespace daw

