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

#include <sstream>
#include <string>
#include <utility>

#include "glucose_unit.h"

namespace daw {
	namespace glucose {
		namespace {
			constexpr double to_mg_dL( mmol_L const & glucose ) noexcept {
				return glucose.value / 0.0555;
			}

			constexpr double to_mmol_L( mg_dL const & glucose ) noexcept {
				return glucose.value * 0.0555;
			}
		}	// namespace anonymous

		mmol_L::mmol_L( double d ) noexcept:
				value{ std::move( d ) } { }

		mmol_L::mmol_L( mg_dL const & g ) noexcept:
				value{ to_mmol_L( g ) } { }

		mmol_L::operator double( ) noexcept {
			return value;
		}

		mmol_L & mmol_L::operator=( mg_dL const & rhs ) noexcept {
			value = to_mmol_L( rhs ); 
		}

		mmol_L::~mmol_L( ) { }

		void swap( mmol_L & lhs, mmol_L & rhs ) noexcept {
			using std::swap;
			swap( lhs.value, rhs.value );
		}

		std::string mmol_L::to_string( ) const {
			std::stringstream ss;
			ss << value << "mmol/L";
			return ss.str( );
		}

		std::ostream & operator<<( std::ostream & os, mmol_L const & glucose ) {
			os << glucose.to_string( );
			return os;
		}

		mmol_L & mmol_L::operator+=( mmol_L const & rhs ) noexcept {
			value += rhs.value;
			return *this;
		}

		mmol_L & mmol_L::operator-=( mmol_L const & rhs ) noexcept {
			value -= rhs.value;
			return *this;
		}

		mmol_L & mmol_L::operator*=( mmol_L const & rhs ) noexcept {
			value *= rhs.value;
			return *this;
		}

		mmol_L & mmol_L::operator/=( mmol_L const & rhs ) noexcept {
			value /= rhs.value;
			return *this;
		}

		mmol_L & mmol_L::operator*=( double const & rhs ) noexcept {
			value *= rhs;
			return *this;
		}

		mmol_L & mmol_L::operator/=( double const & rhs ) noexcept {
			value /= rhs;
			return *this;
		}

		mmol_L operator+( mmol_L const & lhs, mmol_L const & rhs ) noexcept {
			return mmol_L{ lhs.value + rhs.value };
		}

		mmol_L operator-( mmol_L const & lhs, mmol_L const & rhs ) noexcept {
			return mmol_L{ lhs.value - rhs.value };
		}

		mmol_L operator*( mmol_L const & lhs, mmol_L const & rhs ) noexcept {
			return mmol_L{ lhs.value * rhs.value };
		}

		mmol_L operator/( mmol_L const & lhs, mmol_L const & rhs ) noexcept {
			return mmol_L{ lhs.value / rhs.value };
		}

		mmol_L operator*( mmol_L const & lhs, double const & rhs ) noexcept {
			return mmol_L{ lhs.value * rhs };
		}

		mmol_L operator/( mmol_L const & lhs, double const & rhs ) noexcept {
			return mmol_L{ lhs.value * rhs };
		}

		mmol_L operator-( mmol_L rhs ) noexcept {
			rhs.value *= -1.0; 
			return rhs;
		}

		bool operator==( mmol_L const & lhs, mmol_L const & rhs ) noexcept {
			return lhs.value == rhs.value;
		}

		bool operator!=( mmol_L const & lhs, mmol_L const & rhs ) noexcept {
			return lhs.value != rhs.value;
		}

		bool operator<( mmol_L const & lhs, mmol_L const & rhs ) noexcept {
			return lhs.value < rhs.value;
		}

		bool operator>( mmol_L const & lhs, mmol_L const & rhs ) noexcept {
			return lhs.value > rhs.value;
		}

		bool operator<=( mmol_L const & lhs, mmol_L const & rhs ) noexcept {
			return lhs.value <= rhs.value;
		}

		bool operator>=( mmol_L const & lhs, mmol_L const & rhs ) noexcept {
			return lhs.value >= rhs.value;
		}

		mg_dL::mg_dL( double d ) noexcept:
				value{ std::move( d ) } { }

		mg_dL::mg_dL( mmol_L const & g ) noexcept:
				value{ to_mg_dL( g ) } { }

		mg_dL::~mg_dL( ) { }

		mg_dL::operator double( ) noexcept {
			return value;
		}
		
		mg_dL & mg_dL::operator=( mmol_L const & rhs ) noexcept {
			value = to_mg_dL( rhs ); 
		}

		void swap( mg_dL & lhs, mg_dL & rhs ) noexcept {
			using std::swap;
			swap( lhs.value, rhs.value );
		}

		std::string mg_dL::to_string( ) const {
			std::stringstream ss;
			ss << value << "mg/dL";
			return ss.str( );
		}

		std::ostream & operator<<( std::ostream & os, mg_dL const & glucose ) {
			os << glucose.to_string( );
			return os;
		}

		mg_dL & mg_dL::operator+=( mg_dL const & rhs ) noexcept {
			value += rhs.value;
			return *this;
		}

		mg_dL & mg_dL::operator-=( mg_dL const & rhs ) noexcept {
			value -= rhs.value;
			return *this;
		}

		mg_dL & mg_dL::operator*=( mg_dL const & rhs ) noexcept {
			value *= rhs.value;
			return *this;
		}

		mg_dL & mg_dL::operator/=( mg_dL const & rhs ) noexcept {
			value /= rhs.value;
			return *this;
		}

		mg_dL & mg_dL::operator*=( double const & rhs ) noexcept {
			value *= rhs;
			return *this;
		}

		mg_dL & mg_dL::operator/=( double const & rhs ) noexcept {
			value /= rhs;
			return *this;
		}

		mg_dL operator+( mg_dL const & lhs, mg_dL const & rhs ) noexcept {
			return mg_dL{ lhs.value + rhs.value };
		}

		mg_dL operator-( mg_dL const & lhs, mg_dL const & rhs ) noexcept {
			return mg_dL{ lhs.value - rhs.value };
		}

		mg_dL operator*( mg_dL const & lhs, mg_dL const & rhs ) noexcept {
			return mg_dL{ lhs.value * rhs.value };
		}

		mg_dL operator/( mg_dL const & lhs, mg_dL const & rhs ) noexcept {
			return mg_dL{ lhs.value / rhs.value };
		}

		mg_dL operator*( mg_dL const & lhs, double const & rhs ) noexcept {
			return mg_dL{ lhs.value * rhs };
		}

		mg_dL operator/( mg_dL const & lhs, double const & rhs ) noexcept {
			return mg_dL{ lhs.value * rhs };
		}

		mg_dL operator-( mg_dL rhs ) noexcept {
			rhs.value *= -1.0; 
			return rhs;
		}

		bool operator==( mg_dL const & lhs, mg_dL const & rhs ) noexcept {
			return lhs.value == rhs.value;
		}

		bool operator!=( mg_dL const & lhs, mg_dL const & rhs ) noexcept {
			return lhs.value != rhs.value;
		}

		bool operator<( mg_dL const & lhs, mg_dL const & rhs ) noexcept {
			return lhs.value < rhs.value;
		}

		bool operator>( mg_dL const & lhs, mg_dL const & rhs ) noexcept {
			return lhs.value > rhs.value;
		}

		bool operator<=( mg_dL const & lhs, mg_dL const & rhs ) noexcept {
			return lhs.value <= rhs.value;
		}

		bool operator>=( mg_dL const & lhs, mg_dL const & rhs ) noexcept {
			return lhs.value >= rhs.value;
		}
	}	// namespace glucose

}  // namespace daw
 
daw::glucose::mmol_L operator"" _mmol_L( long double d ) noexcept {
	return daw::glucose::mmol_L{ static_cast<double>( d ) };
}

daw::glucose::mg_dL operator"" _mg_dL( long double d ) noexcept {
	return daw::glucose::mg_dL{ static_cast<double>( d ) };
}

daw::glucose::mmol_L operator"" _mmol_L( unsigned long long i ) noexcept {
	return daw::glucose::mmol_L{ static_cast<double>( i ) };
}

daw::glucose::mg_dL operator"" _mg_dL( unsigned long long i ) noexcept {
	return daw::glucose::mg_dL{ static_cast<double>( i ) };
}

