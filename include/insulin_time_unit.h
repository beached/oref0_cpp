
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
#include "insulin_unit.h"

namespace ns {
	template<size_t SECONDS>
	struct insulin_per_SECONDS_t{
		insulin_t value;

		explicit insulin_per_SECONDS_t( insulin_t insulin ) noexcept:
				value{ std::move( insulin ) } { }		

		template<size_t UNIT_TIME>
		insulin_per_SECONDS_t( insulin_per_SECONDS_t<UNIT_TIME> const & other ) noexcept:
				value{ other.per( SECONDS ) } { }

		template<size_t UNIT_TIME>
		insulin_per_SECONDS_t & operator=( insulin_per_SECONDS_t<UNIT_TIME> const & other ) noexcept {
			value = other.per( SECONDS );
		}

		~insulin_per_SECONDS_t( ) = default;

		insulin_per_SECONDS_t( ) = default;
		insulin_per_SECONDS_t( insulin_per_SECONDS_t const & ) = default;
		insulin_per_SECONDS_t( insulin_per_SECONDS_t && ) = default;
		insulin_per_SECONDS_t & operator=( insulin_per_SECONDS_t const & ) = default;
		insulin_per_SECONDS_t & operator=( insulin_per_SECONDS_t && ) = default;

		friend void swap( insulin_per_SECONDS_t & lhs, insulin_per_SECONDS_t & rhs ) noexcept {
			using std::swap;
			swap( lhs.value, rhs.value );
		}

		std::string to_string( ) const {
			std::stringstream ss;
			ss << value;
			switch( SECONDS ) {
			case 60:
				ss << "/min";
				break;
			case 3600:
				ss << "/hr";
				break;
			default:
				ss << "/" << SECONDS << "min";
				break;
			}
			return ss.str( );		
		}

		insulin_per_SECONDS_t & scale( double factor ) noexcept {
			value.scale( factor );
			return *this;
		}

		insulin_per_SECONDS_t scale( double factor ) const noexcept {
			insulin_per_SECONDS_t result{ *this };
			result.scale( factor );
			return result;
		}

		template<typename... Args>
		insulin_t per( std::chrono::duration<Args...> duration ) const {
			auto result = value;
			auto const factor = static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(duration).count( )/SECONDS);
			value.scale( factor );
			return value;
		}
	};	// insulin_per_SECONDS_t

	using insulin_per_hour_t = insulin_per_SECONDS_t<3600>;

	template<typename... Args>
	insulin_per_hour_t operator/( insulin_t const & lhs, std::chrono::duration<Args...> const & rhs ) noexcept {
		double const tp = std::chrono::duration_cast<std::chrono::seconds>( rhs ).count( );
		return insulin_per_hour_t{ lhs.scale( tp / 3600.0 ) };
	}

	template<size_t SECONDS>
	std::ostream & operator<<( std::ostream & os, insulin_per_SECONDS_t<SECONDS> const & insulin_rate ) {
		os << insulin_rate.to_string( );
		return os;
	}
}    // namespace ns

ns::insulin_per_hour_t operator"" _U_hr( long double d ) noexcept;
ns::insulin_per_hour_t operator"" _U_hr( unsigned long long i ) noexcept;
