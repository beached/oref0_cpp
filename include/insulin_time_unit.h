
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
	template<size_t MINUTES>
	struct insulin_per_MINUTES_t{
		insulin_t value;

		explicit insulin_per_MINUTES_t( insulin_t insulin ) noexcept:
				value{ std::move( insulin ) } { }		

		~insulin_per_MINUTES_t( ) = default;

		insulin_per_MINUTES_t( ) = default;
		insulin_per_MINUTES_t( insulin_per_MINUTES_t const & ) = default;
		insulin_per_MINUTES_t( insulin_per_MINUTES_t && ) = default;
		insulin_per_MINUTES_t & operator=( insulin_per_MINUTES_t const & ) = default;
		insulin_per_MINUTES_t & operator=( insulin_per_MINUTES_t && ) = default;

		friend void swap( insulin_per_MINUTES_t & lhs, insulin_per_MINUTES_t & rhs ) noexcept {
			using std::swap;
			swap( lhs.value, rhs.value );
		}

		std::string to_string( ) const {
			std::stringstream ss;
			ss << value;
			switch( MINUTES ) {
			case 1:
				ss << "/min";
				break;
			case 60:
				ss << "/hr";
				break;
			default:
				ss << "/" << MINUTES << "min";
				break;
			}
			return ss.str( );		
		}

		insulin_per_MINUTES_t & scale( double factor ) noexcept {
			value.scale( factor );
			return *this;
		}

		insulin_per_MINUTES_t scale( double factor ) const noexcept {
			insulin_per_MINUTES_t result{ *this };
			result.scale( factor );
			return result;
		}

		insulin_t per( std::chrono::minutes duration ) const {
			auto result = value;
			auto const factor = static_cast<double>(duration.count( )/MINUTES);
			value.scale( factor );
			return value;
		}
	};	// insulin_per_MINUTES_t

	using insulin_per_hour_t = insulin_per_MINUTES_t<60>;
	using insulin_per_minute_t = insulin_per_MINUTES_t<1>;

	template<size_t MINUTES>
	std::ostream & operator<<( std::ostream & os, insulin_per_MINUTES_t<MINUTES> const & insulin_rate ) {
		os << insulin_rate.to_string( );
		return os;
	}
}    // namespace ns

ns::insulin_per_hour_t operator"" _U_hr( long double d ) noexcept;
ns::insulin_per_hour_t operator"" _U_hr( unsigned long long i ) noexcept;
ns::insulin_per_minute_t operator"" _U_min( long double d ) noexcept;
ns::insulin_per_minute_t operator"" _U_min( unsigned long long i ) noexcept;
