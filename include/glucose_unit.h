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

namespace daw {
	namespace glucose {
		struct mg_dL;

		struct mmol_L{
			double value;

			explicit mmol_L( double d ) noexcept;
			mmol_L( mg_dL const & g ) noexcept;
			~mmol_L( );
			explicit operator double( ) noexcept;
			mmol_L & operator=( mg_dL const & rhs ) noexcept;

			mmol_L( ) = default;
			mmol_L( mmol_L const & ) = default;
			mmol_L( mmol_L && ) = default;
			mmol_L & operator=( mmol_L const & ) = default;
			mmol_L & operator=( mmol_L && ) = default;
			friend void swap( mmol_L & lhs, mmol_L & rhs ) noexcept;
			std::string to_string( ) const;

			mmol_L & operator+=( mmol_L const & rhs ) noexcept;
			mmol_L & operator-=( mmol_L const & rhs ) noexcept;
			mmol_L & operator*=( mmol_L const & rhs ) noexcept;
			mmol_L & operator/=( mmol_L const & rhs ) noexcept;
			mmol_L & operator*=( double const & rhs ) noexcept;
			mmol_L & operator/=( double const & rhs ) noexcept;
		};	// mmol_L

		void swap( mmol_L & lhs, mmol_L & rhs ) noexcept;
		std::ostream & operator<<( std::ostream & os, mmol_L const & glucose );	

		mmol_L operator+( mmol_L const & lhs, mmol_L const & rhs ) noexcept;
		mmol_L operator-( mmol_L const & lhs, mmol_L const & rhs ) noexcept;
		mmol_L operator*( mmol_L const & lhs, mmol_L const & rhs ) noexcept;
		mmol_L operator/( mmol_L const & lhs, mmol_L const & rhs ) noexcept;
		mmol_L operator*( mmol_L const & lhs, double const & rhs ) noexcept;
		mmol_L operator/( mmol_L const & lhs, double const & rhs ) noexcept;
		mmol_L operator-( mmol_L rhs ) noexcept;
		bool operator==( mmol_L const & lhs, mmol_L const & rhs ) noexcept;
		bool operator!=( mmol_L const & lhs, mmol_L const & rhs ) noexcept;
		bool operator<( mmol_L const & lhs, mmol_L const & rhs ) noexcept;
		bool operator>( mmol_L const & lhs, mmol_L const & rhs ) noexcept;
		bool operator<=( mmol_L const & lhs, mmol_L const & rhs ) noexcept;
		bool operator>=( mmol_L const & lhs, mmol_L const & rhs ) noexcept;

		struct mg_dL {
			double value;

			explicit mg_dL( double d ) noexcept;
			mg_dL( mmol_L const & g ) noexcept;
			~mg_dL( );
			explicit operator double( ) noexcept;
			mg_dL & operator=( mmol_L const & rhs ) noexcept;

			mg_dL( ) = default;
			mg_dL( mg_dL const & ) = default;
			mg_dL( mg_dL && ) = default;
			mg_dL & operator=( mg_dL const & ) = default;
			mg_dL & operator=( mg_dL && ) = default;
			friend void swap( mg_dL & lhs, mg_dL & rhs ) noexcept;
			std::string to_string( ) const;

			mg_dL & operator+=( mg_dL const & rhs ) noexcept;
			mg_dL & operator-=( mg_dL const & rhs ) noexcept;
			mg_dL & operator*=( mg_dL const & rhs ) noexcept;
			mg_dL & operator/=( mg_dL const & rhs ) noexcept;
			mg_dL & operator*=( double const & rhs ) noexcept;
			mg_dL & operator/=( double const & rhs ) noexcept;
		};	// mg_dL

		void swap( mg_dL & lhs, mg_dL & rhs ) noexcept;
		std::ostream & operator<<( std::ostream & os, mg_dL const & glucose );	

		mg_dL operator+( mg_dL const & lhs, mg_dL const & rhs ) noexcept;
		mg_dL operator-( mg_dL const & lhs, mg_dL const & rhs ) noexcept;
		mg_dL operator*( mg_dL const & lhs, mg_dL const & rhs ) noexcept;
		mg_dL operator/( mg_dL const & lhs, mg_dL const & rhs ) noexcept;
		mg_dL operator*( mg_dL const & lhs, double const & rhs ) noexcept;
		mg_dL operator/( mg_dL const & lhs, double const & rhs ) noexcept;
		mg_dL operator-( mg_dL rhs ) noexcept;
		bool operator==( mg_dL const & lhs, mg_dL const & rhs ) noexcept;
		bool operator!=( mg_dL const & lhs, mg_dL const & rhs ) noexcept;
		bool operator<( mg_dL const & lhs, mg_dL const & rhs ) noexcept;
		bool operator>( mg_dL const & lhs, mg_dL const & rhs ) noexcept;
		bool operator<=( mg_dL const & lhs, mg_dL const & rhs ) noexcept;
		bool operator>=( mg_dL const & lhs, mg_dL const & rhs ) noexcept;
	}	// namespace glucose
}    // namespace daw

daw::glucose::mmol_L operator"" _mmol_L( long double d ) noexcept;
daw::glucose::mg_dL operator"" _mg_dL( long double d ) noexcept;
daw::glucose::mmol_L operator"" _mmol_L( unsigned long long i ) noexcept;
daw::glucose::mg_dL operator"" _mg_dL( unsigned long long i ) noexcept;

