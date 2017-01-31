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

#include <boost/optional.hpp>
#include <boost/utility/string_view.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <date/date.h>
#include <date/tz.h>
#include <chrono>

#include <daw/json/daw_json_link.h>

#include "ns_entries.h"
#include "glucose_unit_json.h"

namespace ns {
	namespace data {
		namespace entries {
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

			void ns_entries_t::link_values( ) {
				this->link_string( "_id", id );
				this->link_streamable( "direction", direction );
				ns::json_link_glucose_t( "previousSGV", this, previous_sgv );
				this->link_iso8601_timestamp( "dateString", timestamp );

				this->link_string( "device", device );
				this->link_epoch_milliseconds_timestamp( "date", date );
				ns::json_link_glucose_t( "sgv", this, sgv );
				this->link_streamable( "type", type );
				this->link_boolean( "previousSGVNotActive", previous_sgv_not_active );
			}

			ns_entries_t::ns_entries_t( ):
					daw::json::JsonLink<ns_entries_t>{ },
					id{ },
					direction{ nightscout_direction::not_computable },
					previous_sgv{ },
					timestamp{ },
					device{ },
					date{ },
					sgv{ -1 },
					type{ },
					previous_sgv_not_active{ } {

				link_values( );
			}

			ns_entries_t::~ns_entries_t( ) { }


			ns_entries_t::ns_entries_t( ns_entries_t const & other ):
					daw::json::JsonLink<ns_entries_t>{ },
					id{ other.id },
					direction{ other.direction },
					previous_sgv{ other.previous_sgv },
					timestamp{ other.timestamp },
					device{ other.device },
					date{ other.date },
					sgv{ other.sgv },
					type{ other.type },
					previous_sgv_not_active{ other.previous_sgv_not_active } {

				link_values( );
			}

			ns_entries_t::ns_entries_t( ns_entries_t && other ):
					daw::json::JsonLink<ns_entries_t>{ },
					id{ std::move( other.id ) },
					direction{ std::move( other.direction ) },
					previous_sgv{ std::move( other.previous_sgv ) },
					timestamp{ std::move( other.timestamp ) },
					device{ std::move( other.device ) },
					date{ std::move( other.date ) },
					sgv{ std::move( other.sgv ) },
					type{ std::move( other.type ) },
					previous_sgv_not_active{ std::move( other.previous_sgv_not_active ) } {

				link_values( );
			}

			std::ostream & operator<<( std::ostream & os, entry_type_t const & tb ) {
				using namespace std::string_literals;
				static std::unordered_map<entry_type_t, std::string> const entry_types {
					{ entry_type_t::sgv, "sgv"s },
					{ entry_type_t::normal, "normal"s }
				};
				os << (entry_types.find( tb )->second);
				return os;
			}

			std::istream & operator>>( std::istream & is, entry_type_t & tb ) {
				using namespace std::string_literals;
				static std::unordered_map<std::string, entry_type_t> const entry_types {
					{ "sgv"s, entry_type_t::sgv },
					{ "normal"s, entry_type_t::normal }
				};
				std::string const tmp{ std::istreambuf_iterator<char>{ is }, std::istreambuf_iterator<char>{ } };
				tb = entry_types.find( tmp )->second;
				return is;
			}
		}	// namespace entries
	} 	// namespace data
}	// namespace ns

