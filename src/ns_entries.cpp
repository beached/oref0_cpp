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

#include <boost/optional.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/utility/string_view.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>

#include <daw/json/daw_json_link.h>

#include "ns_entries.h"

namespace ns {
	namespace {
		std::string to_string( nightscout_direction dir ) noexcept {
			switch( dir ) {
				case nightscout_direction::none: return "NONE";
				case nightscout_direction::double_up: return "DoubleUp";
				case nightscout_direction::single_up: return "SingleUp";
				case nightscout_direction::forty_five_up: return "FortyFiveUp"; 
				case nightscout_direction::flat: return "Flat";
				case nightscout_direction::forty_five_down: return "FortyFiveDown";
				case nightscout_direction::single_down: return "SingleDown";
				case nightscout_direction::double_down: return "DoubleDown";
				case nightscout_direction::not_computable: return "NOT COMPUTABLE";
				case nightscout_direction::rate_out_of_range: return "RATE OUT OF RANGE";
			}
			std::abort( );
		}

		nightscout_direction parse_direction( boost::string_view str ) {
			static std::unordered_map<std::string, nightscout_direction> const results = { 
				{ "NONE", nightscout_direction::none },
				{ "DoubleUp", nightscout_direction::double_up },
				{ "SingleUp", nightscout_direction::single_up },
				{ "FortyFiveUp", nightscout_direction::forty_five_up },
				{ "Flat", nightscout_direction::flat },
				{ "FortyFiveDown", nightscout_direction::forty_five_down },
				{ "SingleDown", nightscout_direction::single_down },
				{ "DoubleDown", nightscout_direction::double_down },
				{ "NOT COMPUTABLE", nightscout_direction::not_computable },
				{ "RATE OUT OF RANGE", nightscout_direction::rate_out_of_range } 
			};
			return results.at( str.to_string( ) );
		}
	}	// namespace anonymous

	std::ostream & operator<<( std::ostream & os, nightscout_direction dir ) {
		os << to_string( dir );
		return os;
	}

	std::istream & operator>>( std::istream & is, nightscout_direction & dir ) {
		std::string tmp;
		is >> tmp;
		dir = parse_direction( tmp );
		return is;
	}

	nightscout_entries::nightscout_entries( ):
			daw::json::JsonLink<nightscout_entries>{ },
			id{ },
			direction{ nightscout_direction::not_computable },
			previous_sgv{ },
			timestamp{ },
			device{ },
			date{ -1 },
			sgv{ -1 },
			type{ },
			previous_sgv_not_active{ } {

		link_string( "_id", id );
		link_streamable( "direction", direction );
		link_integral( "previousSGV", previous_sgv );
		//link_timestamp( "dateString", timestamp );
		link_string( "device", device );
		link_integral( "date", date );
		link_integral( "sgv", sgv );
		link_string( "type", type );
		link_boolean( "previousSGVNotActive", previous_sgv_not_active );
	}

	nightscout_entries::~nightscout_entries( ) { }

	nightscout_entries::nightscout_entries( nightscout_entries const & other ):
			daw::json::JsonLink<nightscout_entries>{ },
			id{ other.id },
			direction{ other.direction },
			previous_sgv{ other.previous_sgv },
			timestamp{ other.timestamp },
			device{ other.device },
			date{ other.date },
			sgv{ other.sgv },
			type{ other.type },
			previous_sgv_not_active{ other.previous_sgv_not_active } {

		link_string( "_id", id );
		link_streamable( "direction", direction );
		link_integral( "previousSGV", previous_sgv );
		//link_timestamp( "dateString", timestamp );
		link_string( "device", device );
		link_integral( "date", date );
		link_integral( "sgv", sgv );
		link_string( "type", type );
		link_boolean( "previousSGVNotActive", previous_sgv_not_active );
	}

	nightscout_entries & nightscout_entries::operator=( nightscout_entries const & rhs ) {
		if( this != &rhs ) {
			nightscout_entries tmp{ rhs };
			using std::swap;
			swap( *this, tmp );
		}
		return *this;
	}	
}

