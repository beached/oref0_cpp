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

#include <daw/daw_string_view.h>
#include <daw/json/daw_json_link.h>

#include "data_types.h"

namespace ns {
	constexpr real_t to_mg_dL( real_t const & glucose ) noexcept {
		return glucose / 0.0555;
	}

	constexpr real_t to_mmol_L( real_t const & glucose ) noexcept {
		return glucose * 0.0555;
	}		

	enum class glucose_unit { mmol_L, mg_dL };

	glucose_unit get_default_glucose_display_unit( ) noexcept;
	void set_default_glucose_display_unit( glucose_unit unit ) noexcept;

	struct glucose_t {
		real_t value;

		constexpr glucose_t( real_t d ) noexcept : value{std::move( d )} {}

		constexpr glucose_t( real_t d, glucose_unit unit ) noexcept
		    : value{unit == glucose_unit::mg_dL ? d : to_mg_dL( d )} {}

		explicit constexpr operator real_t( ) noexcept {
			return value;
		}

		~glucose_t( ) noexcept = default;
		constexpr glucose_t( glucose_t const & ) = default;
		constexpr glucose_t( glucose_t && ) noexcept = default;
		constexpr glucose_t & operator=( glucose_t const & ) = default;
		constexpr glucose_t & operator=( glucose_t && ) noexcept = default;
	
		std::string to_string( ) const;
		std::string to_string( glucose_unit unit ) const;

		constexpr glucose_t &operator+=( glucose_t const &rhs ) noexcept {
			value += rhs.value;
			return *this;
		}

		constexpr glucose_t &operator-=( glucose_t const &rhs ) noexcept {
			value -= rhs.value;
			return *this;
		}

		constexpr glucose_t &scale( real_t factor ) noexcept {
			value *= factor;
			return *this;
		}

		constexpr glucose_t scale( real_t factor ) const noexcept {
			glucose_t result{*this};
			result.value *= factor;
			return result;
		}

		constexpr glucose_t &operator-( ) noexcept {
			value *= -1.0;
			return *this;
		}

		constexpr glucose_t operator-( ) const noexcept {
			glucose_t result{*this};
			result.value *= -1.0;
			return result;
		}

		constexpr real_t as_mmol_L( ) const noexcept {
			return to_mmol_L( value );
		}

		constexpr real_t as_mg_dL( ) const noexcept {
			return value;
		}
	};	// glucose_t

	std::string to_string( glucose_t const & g );
	std::string to_string( glucose_t const & g, glucose_unit unit );

	constexpr void swap( glucose_t &lhs, glucose_t &rhs ) noexcept {
		auto tmp = lhs.value;
		lhs.value = rhs.value;
		rhs.value = tmp;
	}

	constexpr glucose_t mmol_L( real_t d ) noexcept {
		return glucose_t{ to_mg_dL( d ) };
	}

	constexpr glucose_t mg_dL( real_t d ) noexcept {
		return glucose_t{ d };
	}

	constexpr glucose_t operator+( glucose_t lhs, glucose_t const & rhs ) noexcept {
		return lhs += rhs;
	}

	constexpr glucose_t operator-( glucose_t lhs, glucose_t const & rhs ) noexcept {
		return lhs -= rhs;
	}

	constexpr bool operator==( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return lhs.value == rhs.value;
	}

	constexpr bool operator!=( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return lhs.value != rhs.value;
	}

	constexpr bool operator<( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return lhs.value < rhs.value;
	}

	constexpr bool operator>( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return lhs.value > rhs.value;
	}

	constexpr bool operator<=( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return lhs.value <= rhs.value;
	}

	constexpr bool operator>=( glucose_t const & lhs, glucose_t const & rhs ) noexcept {
		return lhs.value >= rhs.value;
	}

	std::ostream & operator<<( std::ostream & os, glucose_t const & glucose );	

	template<typename Object, typename Member>
	void json_link_glucose_t( daw::string_view json_name, Object * entries, Member & member ) {
		using json_int_t = int64_t;
		static auto const to_glucose = []( auto const & json_int ) {
			return ns::glucose_t{ static_cast<ns::real_t>( json_int ) };
		};
		static auto const from_glucose = []( auto const & g ) {
			return static_cast<json_int_t>( g.as_mg_dL( ) );
		};
		entries->link_jsonintegral( json_name, member, from_glucose, to_glucose );
	}

	template<typename Object, typename Member>
	void json_link_glucose_t( daw::string_view json_name, Object * entries, boost::optional<Member> & member ) {
		using json_int_t = int64_t;
		static auto const to_glucose = []( auto const & json_int ) {
			return ns::glucose_t{ static_cast<double>( json_int ) };
		};
		static auto const from_glucose = []( auto const & g ) -> boost::optional<json_int_t> {
			if( !g ) {
				return boost::none;
			}
			return static_cast<json_int_t>( g->as_mg_dL( ) );
		};
		entries->link_jsonintegral( json_name, member, from_glucose, to_glucose );
	}
}	// namespace ns

constexpr ns::glucose_t operator"" _mmol_L( long double d ) noexcept {
	return ns::glucose_t{ns::to_mg_dL( static_cast<double>( d ) )};
}

constexpr ns::glucose_t operator"" _mg_dL( long double d ) noexcept {
	return ns::glucose_t{ static_cast<double>( d ) };
}

constexpr ns::glucose_t operator"" _mmol_L( unsigned long long i ) noexcept {
	return ns::glucose_t{ ns::to_mg_dL( i ) };
}

constexpr ns::glucose_t operator"" _mg_dL( unsigned long long i ) noexcept {
	return ns::glucose_t{ static_cast<double>( i ) };
}

