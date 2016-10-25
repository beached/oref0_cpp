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
#include <random>

#include "iob_calc.h"

size_t random_generator(size_t min, size_t max) {
	static static auto rng = []( ) {
		std::mt19937 result; 
		result.seed(std::random_device()());
		result.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		return result;
	}( );
	std::uniform_int_distribution<std::mt19937::result_type> dist( min, max );

	return dist( rng );
}

struct carb_doses_t {
	double amount;
	double absorption_rate;
	std::chrono::time_point<std::chrono::system_clock> dose_time;
};
///
// Calculate how many g/carb are left in dose with a linear model of absoption
// absorption_rate is in g/min
double calc_carb_left( std::chrono::time_point<std::chrono::system_clock> const & ts_now, carb_doses_t const & carb ) {
	auto const duration = std::chrono::duration_cast<std::chrono::seconds>( ts_now - carb.dose_time ).count( );
	auto const abs_carbs = duration * carb.absorption_rate;
	if( abs_carbs >= carb.amount ) {
		return 0;
	}
	return carb.amount - abs_carbs;
}

void clean_up( std::vector<ns::dose> & insulin_doses, std::vector<carb_doses_t> & carb_doses ) {
	auto const ts_now = std::chrono::system_clock::now( ); 
	insulin_doses.erase( std::remove_if( insulin_doses.begin( ), insulin_doses.end( ), [&]( auto const & item ) {
		auto const duration = std::chrono::duration_cast<std::chrono::seconds>( ts_now - item.dose_time ).count( );
		return duration >= static_cast<int16_t>(item.dose_dia);
	} ), insulin_doses.end( ) );

	carb_doses.erase( std::remove_if( carb_doses.begin( ), carb_doses.end( ), [&]( auto const & item ) {
		return calc_carb_left( ts_now, item ) <= 0.0; 
	} ), carb_doses.end( ) );
}


int main( int, char ** ) {
	using namespace std::chrono;
	using namespace date;
	double const basal_dose_per_hr = 1.2;
	double const basal_dose_per_5min = basal_dose_per_hr/12.0;
	std::vector<ns::dose> insulin_doses;
	std::vector<carb_doses_t> carb_doses;
	double last_iob = 0.0;
	double last_cob = 0.0;
	double glucose_delta = 0.0;
	double const isf = 3.5;
	double const icr = 15.0;

	auto const calc_iob = []( auto const & ts_now, auto const & value ) {
		auto const duration = std::chrono::duration_cast<std::chrono::seconds>( ts_now - value.dose_time ).count( );
		if( duration > static_cast<int16_t>(value.dose_dia) ) {
			return 0.0;
		}
		auto const pc = ns::insulin_on_board_pct( duration, value.dose_dia );
		std::cerr << " dose_dia=" << static_cast<int16_t>(value.dose_dia) << " age=" << duration << " %=" << pc << " amount=" << value.amount << " value=" << (pc*value.amount) << '\n';
		return value.amount * pc;
	};

	auto const add_insulin_dose = [&last_iob, &insulin_doses]( double amount, ns::insulin_duration_t dia = ns::insulin_duration_t::t240, ns::dose::timestamp_t when = system_clock::now( ) ) {
		std::cout << "insulin_dose: " << amount << "U\n";
		last_iob += amount;
		insulin_doses.push_back( { amount, dia, std::move( when ) } );
	};

	auto const add_carb_dose = [&last_cob, &carb_doses]( double amount, double absorption_rate = 0.25, std::chrono::time_point<std::chrono::system_clock> when = system_clock::now( ) ) {
		std::cout << "carb_dose: " << amount << "U\n";
		last_cob += amount;
		carb_doses_t c;
		c.amount = amount;
		c.dose_time = when;
		c.absorption_rate = absorption_rate;
		carb_doses.push_back( std::move( c ) );
	};

	auto const add_insulin_carb_dose = [&] ( double carb_amount ) {
		auto const insulin_dose = carb_amount/icr;
		add_insulin_dose( insulin_dose );
		add_carb_dose( carb_amount, 0.8 );
	};

	auto const ts_start = std::chrono::system_clock::now( ); 
	size_t last_duration = 0;
	bool is_first = true;

	add_insulin_carb_dose( 50 );

	while( true ) {
		auto const ts_now = std::chrono::system_clock::now( ); 
		auto const cur_duration = std::chrono::duration_cast<std::chrono::seconds>( ts_now - ts_start ).count( );

		auto const iob = std::accumulate( insulin_doses.begin( ), insulin_doses.end( ), 0.0, [&]( auto const & init, auto const & value ) {
			return init + calc_iob( ts_now, value );
		} );

		auto const cob = std::accumulate( carb_doses.begin( ), carb_doses.end( ), 0.0, [&]( auto const & init, auto const & value ) {
			return init + calc_carb_left( ts_now, value );
		} );

		auto const iob_diff = last_iob - iob; 
		auto const cob_diff = last_cob - cob;
		last_iob = iob;
		last_cob = cob;

/*		if( cur_duration > last_duration ) {
			double const basal_dose = basal_dose_per_5min*((cur_duration-last_duration)/5.0);
			std::cout << "basal_dose: " << basal_dose << "U\n";
			add_insulin_dose( basal_dose, ns::insulin_duration_t::t180 );
			last_duration = cur_duration;
		}
		if( random_generator( 0, 120 ) < 5 ) {
			auto const bolus_dose = static_cast<double>(random_generator( 1, 50 ))/10.0;
			add_insulin_dose( bolus_dose, ns::insulin_duration_t::t180 );
		}*/
		double const insulin_drop = iob_diff * isf;
		double const carb_rise = (cob_diff / icr) * isf;
		glucose_delta = glucose_delta + carb_rise - insulin_drop;
		std::cout << ts_now << " t=" << cur_duration << " iob=" << iob << " active_ins=" << iob_diff;
		std::cout << " cob=" << cob << " active_carb=" << cob_diff << " insulin_drop=" << insulin_drop << "mmol/L ";
		std::cout << " carb_rise=" << carb_rise << "mmol/L glucose_delta=" << glucose_delta << "mmol/L\n";
		clean_up( insulin_doses, carb_doses );
		using namespace std::chrono_literals;
		std::this_thread::sleep_for( 5s );
	}
	return 0;
}

