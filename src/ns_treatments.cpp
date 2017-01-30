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

namespace ns {
	namespace data {
		namespace treatments {
			ns_treatments_t::ns_treatments_t( ):
					daw::json::JsonLink<ns_treatments_t>{ },
					created_at{ },
					temp{ },
					insulin{ },
					eventType{ },
					carbs{ },
					_id{ },
					timestamp{ },
					duration{ },
					enteredBy{ },
					absolute{ },
					rate{ } {

				set_links( );
			}

			ns_treatments_t::ns_treatments_t( ns_treatments_t const & other ):
					daw::json::JsonLink<ns_treatments_t>{ },
					created_at{ other.created_at },
					temp{ other.temp },
					insulin{ other.insulin },
					eventType{ other.eventType },
					carbs{ other.carbs },
					_id{ other._id },
					timestamp{ other.timestamp },
					duration{ other.duration },
					enteredBy{ other.enteredBy },
					absolute{ other.absolute },
					rate{ other.rate } {

				set_links( );
			}

			ns_treatments_t::ns_treatments_t( ns_treatments_t && other ):
					daw::json::JsonLink<ns_treatments_t>{ },
					created_at{ std::move( other.created_at ) },
					temp{ std::move( other.temp ) },
					insulin{ std::move( other.insulin ) },
					eventType{ std::move( other.eventType ) },
					carbs{ std::move( other.carbs ) },
					_id{ std::move( other._id ) },
					timestamp{ std::move( other.timestamp ) },
					duration{ std::move( other.duration ) },
					enteredBy{ std::move( other.enteredBy ) },
					absolute{ std::move( other.absolute ) },
					rate{ std::move( other.rate ) } {

				set_links( );
			}

			ns_treatments_t::~ns_treatments_t( ) { }

			void ns_treatments_t::set_links( ) {
				link_string( "created_at", created_at );
				link_string( "temp", temp );
				link_real( "insulin", insulin );
				link_string( "eventType", eventType );
				link_integral( "carbs", carbs );
				link_string( "_id", _id );
				link_string( "timestamp", timestamp );
				link_integral( "duration", duration );
				link_string( "enteredBy", enteredBy );
				link_real( "absolute", absolute );
				link_real( "rate", rate );
			}
		}	// namespace treatmetns
	}	// namespace data
}	// namespace ns
