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

#include <boost/optional.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include <daw/json/daw_json_link.h>

#include "ns_treatments.h"
#include "carb_unit_json.h"
#include "insulin_unit_json.h"
#include "insulin_time_unit_json.h"
#include "time_json.h"

namespace ns {
	namespace data {
		namespace treatments {
			ns_treatments_t::ns_treatments_t( ):
					daw::json::JsonLink<ns_treatments_t>{ },
					created_at{ },
					temp{ },
					insulin{ },
					glucose_type{ },
					eventType{ },
					carbs{ },
					_id{ },
					timestamp{ },
					duration{ },
					enteredBy{ },
					absolute{ },
					rate{ },
					glucose{ },
					units{ },
					absorption_time{ },
					programmed{ },
					ratio{ },
					unabsorbed{ } {

				set_links( );
			}

			ns_treatments_t::ns_treatments_t( ns_treatments_t const & other ):
					daw::json::JsonLink<ns_treatments_t>{ },
					created_at{ other.created_at },
					temp{ other.temp },
					insulin{ other.insulin },
					glucose_type{ other.glucose_type },
					eventType{ other.eventType },
					carbs{ other.carbs },
					_id{ other._id },
					timestamp{ other.timestamp },
					duration{ other.duration },
					enteredBy{ other.enteredBy },
					absolute{ other.absolute },
					rate{ other.rate },
					glucose{ other.glucose },
					units{ other.units },
					absorption_time{ other.absorption_time },
					programmed{ other.programmed },
					ratio{ other.ratio },
					unabsorbed{ other.unabsorbed } {

				set_links( );
			}

			ns_treatments_t::ns_treatments_t( ns_treatments_t && other ):
					daw::json::JsonLink<ns_treatments_t>{ },
					created_at{ std::move( other.created_at ) },
					temp{ std::move( other.temp ) },
					insulin{ std::move( other.insulin ) },
					glucose_type{ std::move( other.glucose_type ) },
					eventType{ std::move( other.eventType ) },
					carbs{ std::move( other.carbs ) },
					_id{ std::move( other._id ) },
					timestamp{ std::move( other.timestamp ) },
					duration{ std::move( other.duration ) },
					enteredBy{ std::move( other.enteredBy ) },
					absolute{ std::move( other.absolute ) },
					rate{ std::move( other.rate ) },
					glucose{ std::move( other.glucose ) },
					units{ std::move( other.units ) },
					absorption_time{ std::move( other.absorption_time ) },
					programmed{ std::move( other.programmed ) },
					ratio{ std::move( other.ratio ) },
					unabsorbed{ std::move( other.unabsorbed ) } {

				set_links( );
			}

			ns_treatments_t::~ns_treatments_t( ) { }

			void ns_treatments_t::set_links( ) {
				link_iso8601_timestamp( "created_at", created_at );
				link_streamable( "temp", temp );
				ns::json_link_insulin_t( "insulin", this, insulin );
				link_string( "glucoseType", glucose_type );
				link_streamable( "eventType", eventType );
				ns::json_link_carb_t( "carbs", this, carbs );
				link_string( "_id", _id );
				link_iso8601_timestamp( "timestamp", timestamp );
				ns::impl::link_int_duration( "duration", this, duration );
				link_string( "enteredBy", enteredBy );
				ns::json_link_insulin_rate_t( "absolute", this, absolute );
				link_real( "rate", rate );
				link_real( "glucose", glucose );
				link_string( "units", units );
				ns::impl::link_int_duration( "absorptionTime", this, absorption_time );
				link_real( "programmed", programmed );
				link_real( "ratio", ratio );
				link_real( "unabsorbed", unabsorbed );
			}

			std::ostream & operator<<( std::ostream & os, temp_basal_t const & tb ) {
				using namespace ns::data::treatments;
				using namespace std::string_literals;
				static std::unordered_map<temp_basal_t, std::string> const temp_basals {
					{ temp_basal_t::absolute, "absolute"s },
					{ temp_basal_t::percentage, "percentage"s }
				};
				os << (temp_basals.find( tb )->second);
				return os;
			}

			std::istream & operator>>( std::istream & is, temp_basal_t & tb ) {
				using namespace ns::data::treatments;
				using namespace std::string_literals;
				static std::unordered_map<std::string, temp_basal_t> const temp_basals {
					{ "absolute"s, temp_basal_t::absolute },
					{ "percentage"s, temp_basal_t::percentage }
				};
				std::string const tmp{ std::istreambuf_iterator<char>{ is }, std::istreambuf_iterator<char>{ } };
				tb = temp_basals.find( tmp )->second;
				return is;
			}

			std::ostream & operator<<( std::ostream & os, event_type_t const & tb ) {
				using namespace ns::data::treatments;
				using namespace std::string_literals;
				static std::unordered_map<event_type_t, std::string> const event_types {
					{ event_type_t::temp_basal, "Temp Basal"s },
					{ event_type_t::bg_check, "BG Check"s },
					{ event_type_t::correction_bolus, "Correction Bolus"s },
					{ event_type_t::meal_bolus, "Meal Bolus"s }
				};
				os << (event_types.find( tb )->second);
				return os;
			}

			std::istream & operator>>( std::istream & is, event_type_t & tb ) {
				using namespace ns::data::treatments;
				using namespace std::string_literals;
				static std::unordered_map<std::string, event_type_t> const event_types {
					{ "Temp Basal"s, event_type_t::temp_basal },
					{ "BG Check"s, event_type_t::bg_check },
					{ "Correction Bolus"s, event_type_t::correction_bolus },
					{ "Meal Bolus"s, event_type_t::meal_bolus }
				};
				std::string const tmp{ std::istreambuf_iterator<char>{ is }, std::istreambuf_iterator<char>{ } };
				tb = event_types.find( tmp )->second;
				return is;
			}
		}	// namespace treatments
	}	// namespace data
}	// namespace ns


