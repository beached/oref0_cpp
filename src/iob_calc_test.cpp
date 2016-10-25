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

#include <algorithm>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#include "iob_calc.h"

void clean_up( std::vector<ns::dose> & doses ) {
	auto const ts_now = std::chrono::system_clock::now( ); 
	doses.erase( std::remove_if( doses.begin( ), doses.end( ), [&]( auto const & item ) {
			auto const duration = std::chrono::duration_cast<std::chrono::seconds>( ts_now - item.dose_time ).count( );
			return duration >= static_cast<int16_t>(item.dose_dia);
	} ), doses.end( ) );
}

int main( int, char ** ) {
	using namespace std::chrono;
	using namespace date;
	double const basal_dose_per_hr = 1.2;
	double const basal_dose_per_5min = basal_dose_per_hr/12.0;
	std::vector<ns::dose> doses;
	double last_iob = 0.0;

	auto const add_dose = [&last_iob, &doses]( double amount, ns::insulin_duration_t dia = ns::insulin_duration_t::t240 ) {
		last_iob += amount;
		doses.push_back( { amount, dia } );
	};
		
	add_dose( 5.0, ns::insulin_duration_t::t180 );
	
	auto const ts_start = std::chrono::system_clock::now( ); 
	size_t last_duration = 0;
	bool is_first = true;
	while( true ) {
		auto const ts_now = std::chrono::system_clock::now( ); 
		auto const cur_duration = std::chrono::duration_cast<std::chrono::seconds>( ts_now - ts_start ).count( );

		double const iob = std::accumulate( doses.begin( ), doses.end( ), 0.0, [&ts_now]( auto const & init, auto const & value ) {
				auto const duration = std::chrono::duration_cast<std::chrono::seconds>( ts_now - value.dose_time ).count( );
				//std::cerr << "init=" << init << " duration=" << duration;
				if( duration > static_cast<int16_t>(value.dose_dia) ) {
					return init;
				}
				auto const pc = ns::insulin_on_board_pct( duration, value.dose_dia );
				std::cerr << " dose_dia=" << static_cast<int16_t>(value.dose_dia) << " age=" << duration << " %=" << pc << " amount=" << value.amount << " value=" << (pc*value.amount) << '\n';
				return init + value.amount * pc;
		} );
		double const diff = last_iob - iob; 
		last_iob = iob;

		if( cur_duration > last_duration ) {
			double const basal_dose = basal_dose_per_5min*((cur_duration-last_duration)/5.0);
			std::cout << "basal_dose: " << basal_dose << "U\n";
			add_dose( basal_dose, ns::insulin_duration_t::t180 );
			last_duration = cur_duration;
		}
		std::cout << ts_now << " t=" << cur_duration << " iob=" << iob << " active_ins=" << diff << '\n';
		clean_up( doses );
		using namespace std::chrono_literals;
		std::this_thread::sleep_for( 5s );
	}
	return 0;
}

