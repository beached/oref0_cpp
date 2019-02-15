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

#include <date/date.h>
#include <cstdint>

namespace ns {
	namespace lib {
		namespace iob {
			auto local_tz( ) {
				using namespace date;
				using namespace std::chrono;
				auto const zone = current_zone( );
				auto const now = floor<milliseconds>( system_clock::now( ) );
				auto const local = zone->to_local( now );

				return local.second;
			}
			struct temp_bolus_t {
				timestamp_t timestamp{};
				insulin_t insulin{};

				temp_bolus_t( timestamp_t ts, insulin_t ins ):
						timestamp{ std::move( ts ) },
						insulin{ std::move( ins ) } { }

				temp_bolus_t( ) = default;
			};	// temp_bolus_t

			enum class pump_history_types { Bolus, TempBasal, TempBasalDuration };
			enum class pump_temp_basal_types { Percentage, Absolute };

			template<typename Inputs>
			auto calc_temp_treatments( Inputs const & inputs ) {
				auto const & pump_history = inputs.history;
				auto const & profile_data = inputs.profile;
				auto const tz = local_tz( );
				std::vector<temp_bolus_t> temp_boluses;

				for( index_t i=0; i<pump_history.size( ); ++i ) {
					auto const & current = pump_history[i];
					switch( current.type ) {
					case pump_history_types::Bolus:
						temp_boluses.emplace_back( current.timestamp, current.amount );
						break;
					case pump_history_types::Basal:
						if( current.temp_type == pump_temp_basal_types::Percentage ) {
							continue;
						}
						auto const & rate = current.rate;
						auto const & date = current.date;
						boost::optional<duration_t> duration;
						if( i != 0 && pump_history[i-1].data == current.date && pump_history[i-1].type == pump_history_types::TempBasalDuration ) {
							duration = pump_history[i-1].duration;
						} else if( i+1 < pump_history.size( ) )
						break;
					}
				}
			}
		}	// namespace iob
	}	// namespace lib
}    // namespace ns

