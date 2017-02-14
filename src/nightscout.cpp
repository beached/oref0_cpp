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
// The above copyright notice and this permission notice daw::shall be included in all
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
#include <string>

#include "nightscout.h"
#include "sha1.h"

namespace ns {
	namespace {
		auto tp_to_iso_date_string( ns::timestamp_t const tp ) {
			return date::format( "%FT%TZ", tp );
		}

		auto build_nightscout_date_range( boost::string_view field, ns::timestamp_t const tp_start, ns::timestamp_t const tp_end ) {
			std::stringstream ss;
			ss << "find[" << field.data( ) << "][$gte]=";
			ss << tp_to_iso_date_string( tp_start );
			ss << "&find[" << field.data( ) << "][$lte]=";
			ss << tp_to_iso_date_string( tp_end );
			return ss.str( );
		}

		std::string ns_download( boost::string_view url, ns::autotune_config_t const & config ) {
			daw::curl_wrapper cw;
			if( config.api_key ) {
				cw.add_header( "api-secret", daw::sha1( *config.api_key ) );
			}
			auto result = cw.get_string( url );
			return result;
		}
	}	// namespace anonymous

	ns_profile_data_t ns_get_profiles( ns::autotune_config_t const & config ) {
		auto const profile_string_data = ns_download( config.nightscout_base_url + "/api/v1/profile.json", config );
		return daw::json::array_from_string<ns::data::profiles::ns_profiles_t>( profile_string_data, true );
	}

	ns_entries_data_t ns_get_entries( ns::autotune_config_t const & config, ns::timestamp_t tp_start, ns::timestamp_t tp_end ) {

		auto const ymd1 = date::year_month_day{ date::floor<date::days>( tp_start ) };
		auto const ymd2 = date::year_month_day{ date::floor<date::days>( tp_end ) };
		auto const y_1 = std::to_string( static_cast<int>(ymd1.year( )) );
		auto const y_2 = std::to_string( static_cast<int>(ymd2.year( )) );

		auto const dte_rng = build_nightscout_date_range( "dateString", tp_start, tp_end );
		std::string const url = config.nightscout_base_url + "/api/v1/times/{" + y_1 + ".." + y_2 + "}-{1..12}.json?count=1000000&" + dte_rng;
		auto const profile_string_data = ns_download( url, config );

		return daw::json::array_from_string<ns::data::entries::ns_entries_t>( profile_string_data, true );
	}

	ns_treatments_data_t ns_get_treatments( ns::autotune_config_t const & config, ns::timestamp_t tp_start, ns::timestamp_t tp_end ) {
		auto const dte_rng = build_nightscout_date_range( "created_at", tp_start, tp_end );
		std::string const url = config.nightscout_base_url + "/api/v1/treatments.json?count=1000000&" + dte_rng;
		auto const profile_string_data = ns_download( url, config );

		return daw::json::array_from_string<ns::data::treatments::ns_treatments_t>( profile_string_data, true );
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

std::ostream & operator<<( std::ostream & os, ns::ns_entries_data_t const & entries ) {
	os << "[ ";
	bool is_first = true;
	for( auto const & entry: entries ) {
		if( !is_first ) {
			os << ", ";
		} else {
			is_first = true;
		}
		os << entry;
	}
	os << " ]";
	return os;
}

std::ostream & operator<<( std::ostream & os, ns::ns_treatments_data_t const & treatments ) {
	os << "[ ";
	bool is_first = true;
	for( auto const & treatment: treatments ) {
		if( !is_first ) {
			os << ", ";
		} else {
			is_first = true;
		}
		os << treatment;
	}
	os << " ]";
	return os;
}

