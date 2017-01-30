// The MIT License (MIT)
//
// Copyright (c) 2017 Darrell Wright
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

#include <daw/curl_wrapper.h>
#include <chrono>
#include <date/date.h>

#include "nightscout.h"

namespace ns {
	namespace {
		auto tp_to_iso_date_string( std::chrono::system_clock::time_point const tp ) {
			return date::format( "%FT%TZ", tp );
		}

		auto build_nightscout_date_range( boost::string_view field, std::chrono::system_clock::time_point const tp_start, std::chrono::system_clock::time_point const tp_end ) {
			std::stringstream ss;
			ss << "find\\[" << field.data( ) << "\\]\\[$gte\\]=";
			ss << tp_to_iso_date_string( tp_start );
			ss << "&find\\[" << field.data( ) << "\\]\\[$lte\\]=";
			ss << tp_to_iso_date_string( tp_end );
			return ss.str( );
		}

		template<typename ResultType>
		ResultType get_nightscout_data_for_range( boost::string_view url, boost::string_view field, std::chrono::system_clock::time_point const tp_start, std::chrono::system_clock::time_point const tp_end ) {
			daw::curl_wrapper cw;
			ResultType result;
			std::chrono::system_clock::time_point next_tp;

			for( auto tp = tp_start; tp <= tp_end; ) {
				next_tp = tp + date::days{ 1 };
				auto const dte_rng = build_nightscout_date_range( field, tp, next_tp );
				std::string const url_full = url.to_string( ) + "?count=10000&" + dte_rng;
				auto glucose_string_data = cw.get_string( url_full );
				auto tmp = daw::json::array_from_string<typename ResultType::value_type>( glucose_string_data, true );
				result.insert( std::end( result ), std::begin( tmp ), std::end( tmp ) );
				tp = next_tp;
			}
			return result;
		}
	}

	ns_profile_data_t get_nightscout_profile_data( boost::string_view nightscout_base_url ) {
		daw::curl_wrapper cw;
		auto const profile_string_data = cw.get_string( nightscout_base_url.to_string( ) + "/api/v1/profile.json" );
		return daw::json::array_from_string<ns::data::profiles::ns_profiles_t>( profile_string_data, true );
	}

	ns_glucose_data_t get_nightscout_glucose_data( boost::string_view nightscout_base_url, std::chrono::system_clock::time_point tp_start, std::chrono::system_clock::time_point tp_end ) {
		std::string const url = nightscout_base_url.to_string( ) + "/api/v1/entries.json";
		return get_nightscout_data_for_range<ns_glucose_data_t>( url, "dateString", tp_start, tp_end );
	}

	ns_treatments_data_t get_nightscout_treatments_data( boost::string_view nightscout_base_url, std::chrono::system_clock::time_point tp_start, std::chrono::system_clock::time_point tp_end ) {
		std::string const url = nightscout_base_url.to_string( ) + "/api/v1/treatments.json";
		return get_nightscout_data_for_range<ns_treatments_data_t>( url, "timestamp", tp_start, tp_end );
	}
}	// namespace ns

std::ostream & operator<<( std::ostream & os, ns::ns_profile_data_t const & profiles ) {
	os << "[ ";
	bool is_first = true;
	for( auto const & profile: profiles ) {
		if( !is_first ) {
			os << ", ";
		} else {
			is_first = true;
		}
		os << profile;
	}
	os << " ]";
	return os;
}

std::ostream & operator<<( std::ostream & os, ns::ns_glucose_data_t const & glucoses ) {
	os << "[ ";
	bool is_first = true;
	for( auto const & glucose: glucoses ) {
		if( !is_first ) {
			os << ", ";
		} else {
			is_first = true;
		}
		os << glucose;
	}
	os << " ]";
	return os;
}

std::ostream & operator<<( std::ostream & os, ns::ns_treatments_data_t const & treatmentss ) {
	os << "[ ";
	bool is_first = true;
	for( auto const & treatments: treatmentss ) {
		if( !is_first ) {
			os << ", ";
		} else {
			is_first = true;
		}
		os << treatments;
	}
	os << " ]";
	return os;
}

