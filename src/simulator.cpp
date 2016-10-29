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

#include <daw/json/daw_json_link.h>

#include "iob_calc.h"
#include "glucose_unit.h"

using namespace std::chrono_literals;
using namespace std::chrono;
using namespace date;
using namespace daw::glucose;

struct profile_t {
	double isf;
	double icr;

};	// profile_t

struct glucose_state_t {
	std::vector<mmol_L> values;
	size_t value_capacity;
	mmol_L target_value;

	mmol_L current_value( ) const {
		return values.back( );
	}

	mmol_L previous_value( ) const {
		assert( !values.empty( ) );
		if( values.size( ) == 1 ) {
			return values.front( );
		}
		return *std::prev( values.end( ), 2 );
	}

	void add_value( mmol_L v ) {
		values.push_back( std::move( v ) );
		while( values.size( ) > value_capacity ) {
			values.erase( values.begin( ) );
		}
	}

	glucose_state_t( mmol_L target, size_t capacity = 1 ):
			values{ },
			value_capacity{ capacity },
			target_value{ target } {
			
		assert( capacity > 0 );
		values.push_back( target );
	}
			
	glucose_state_t( ):
			glucose_state_t{ 5.5_mmol_L } { }

	~glucose_state_t( ) = default;
	glucose_state_t( glucose_state_t const & ) = default;
	glucose_state_t( glucose_state_t && ) = default;
	glucose_state_t & operator=( glucose_state_t const & ) = default;
	glucose_state_t & operator=( glucose_state_t && ) = default;
};	// glucose_state_t

size_t random_generator(size_t min, size_t max) {
	static auto rng = []( ) {
		std::mt19937 result; 
		result.seed( std::random_device( )( ) );
		result.seed( static_cast<size_t>(high_resolution_clock::now( ).time_since_epoch( ).count( )) );
		return result;
	}( );
	std::uniform_int_distribution<std::mt19937::result_type> dist( min, max );

	return dist( rng );
}

double calc_duration( time_point<system_clock> const & lhs, time_point<system_clock> const & rhs ) {
	return duration_cast<minutes>( rhs - lhs ).count( );
}

struct carb_dose_t {
	double amount;
	double absorption_rate;
	time_point<system_clock> dose_time;

	carb_dose_t( ) = delete;
	~carb_dose_t( ) = default;
	carb_dose_t( carb_dose_t const & ) = default;
	carb_dose_t & operator=( carb_dose_t const & ) = default;
	carb_dose_t( carb_dose_t && ) = default;
	carb_dose_t & operator=( carb_dose_t && ) = default;

	carb_dose_t( time_point<system_clock> doseTime, double Amount, double absorptionRate ):
			amount{ Amount < 0 ? 0 : Amount },
			absorption_rate{ absorptionRate < 0 ? 0 : absorptionRate },
			dose_time{ doseTime } { }

	static double calc_cob( carb_dose_t const & item, time_point<system_clock> const & current_ts ) {
		// Used formula from https://github.com/Perceptus/GlucoDyn/blob/master/basic_math.pdf
		auto const t = calc_duration( item.dose_time, current_ts );
		if( t < 0 ) {
			return item.amount;
		}
		double const AT = item.amount/item.absorption_rate;
		auto const & D = item.amount;
		auto result = D - [&]( ) { 
			if( t < AT/2.0 ) {
				return (2.0*D*t*t)/(AT*AT);
			} else if( t <= AT ) {
				return ((4.0*D)/AT)*(t - (t*t)/(2*AT)) - D;
			}
			return D;
		}( );
		//std::cout << "~~~~calc_cob: AT=" << AT << " D=" << D << " cob=" << result << " %=" << ((result/item.amount)*100) << '\n';
		return result;
	}

	double tick( time_point<system_clock> const & current_ts ) const {
		return calc_cob( *this, current_ts ); 
	}
};

void clean_up( time_point<system_clock> const & ts_now, std::vector<ns::dose> & insulin_doses, std::vector<carb_dose_t> & carb_doses ) {
	insulin_doses.erase( std::remove_if( insulin_doses.begin( ), insulin_doses.end( ), [&]( auto const & item ) {
		auto const duration = calc_duration( item.dose_time, ts_now );
		return duration >= static_cast<intmax_t>(item.dose_dia);
	} ), insulin_doses.end( ) );

	carb_doses.erase( std::remove_if( carb_doses.begin( ), carb_doses.end( ), [&]( auto const & item ) {
		return ts_now > item.dose_time && item.tick( ts_now ) <= 0;
	} ), carb_doses.end( ) );
}

double calc_iob( time_point<system_clock> const & ts_now, ns::dose const & item ) {
	if( ts_now < item.dose_time ) {	// Ignore future insulin
		return item.amount;
	}
	auto const duration = duration_cast<minutes>( ts_now - item.dose_time ).count( );
	if( duration > static_cast<intmax_t>(item.dose_dia) ) {
		return 0.0;
	}
	auto const pc = ns::insulin_on_board_pct( duration, item.dose_dia );
	//std::cout << "~~~~insulin_dose dia=" << static_cast<int16_t>(item.dose_dia) << " age=" << duration << " %=" << pc << " amount=" << item.amount << " left=" << (pc*item.amount) << '\n';
	return item.amount * pc;
}

int main( int, char ** ) {
	double const basal_dose_per_hr = 1.2;
	profile_t profile { 3.5, 15.0 }; 
	double const est_liver_carb_per_hr = basal_dose_per_hr*profile.icr;
	double const est_liver_carb_per_min = est_liver_carb_per_hr/60.0;
	std::vector<ns::dose> insulin_doses;
	std::vector<carb_dose_t> carb_doses;
	double last_iob = 0.0;
	double last_cob = 0.0;
	glucose_state_t glucose_state;
	mmol_L glucose_delta = 0.05_mmol_L;
	double insulin_offset = 0.0;

	std::cout << "Based on ICR=" << profile.icr << "g Carb/U ISF=" << profile.isf << "mmol/L/U basal_rate=" << basal_dose_per_hr;
	std::cout << "U/hr it is estimated that the liver outputs " << est_liver_carb_per_hr << "g/hr of glucose or " << est_liver_carb_per_min << "g/min\n";
	
	auto const add_insulin_dose = [&last_iob, &insulin_doses]( time_point<system_clock> const & when, double amount, ns::insulin_duration_t dia = ns::insulin_duration_t::t240 ) {
		//std::cout << "~~insulin_dose: " << amount << "U\n";
		last_iob += amount;
		insulin_doses.emplace_back( amount, dia, when );
	};

	auto const add_carb_dose = [&last_cob, &carb_doses]( time_point<system_clock> const & when, double amount, double absorption_rate = 0.5 ) {
		//std::cout << "~~carb_dose: " << amount << "g\n";
		last_cob += amount;
		carb_doses.emplace_back( when, amount, absorption_rate );
	};

	auto const add_insulin_carb_dose = [&] ( time_point<system_clock> const & when, double carb_amount ) {
		add_carb_dose( when, carb_amount );
		auto const insulin_dose = carb_amount/profile.icr;
		add_insulin_dose( when, insulin_dose );
	};

	auto const ts_start = system_clock::now( ); 
	size_t last_duration = 0;

	auto ts_now = system_clock::now( ); 

	add_carb_dose( ts_now + 45min, 50, 0.5 );
	add_insulin_dose( ts_now, 50.0/profile.icr );

	while( true ) {
		auto const cur_duration = duration_cast<minutes>( ts_now - ts_start ).count( );

		auto const iob = std::accumulate( insulin_doses.begin( ), insulin_doses.end( ), 0.0, [&]( auto const & init, auto const & value ) {
			return init + calc_iob( ts_now, value );
		} );

		auto const cob = std::accumulate( carb_doses.begin( ), carb_doses.end( ), 0.0, [&]( auto const & init, auto const & value ) {
			return init + value.tick( ts_now );
		} );

		auto const iob_diff = last_iob - iob; 
		auto const cob_diff = last_cob - cob;
		last_iob = iob;
		last_cob = cob;

		if( cur_duration > last_duration ) {
			auto const carb_dose = est_liver_carb_per_min*(cur_duration-last_duration);
			last_duration = cur_duration;

			add_carb_dose( ts_now, carb_dose, carb_dose/180.0 );
			auto ins_dose = carb_dose/profile.icr;
			if( insulin_offset < -0.2 ) {
				std::cout << "Lowering basal to offset too much insulin(TMI): need " << insulin_offset << "U and have " << ins_dose << " to give\n";
				if( ins_dose >= insulin_offset ) {
					ins_dose -= insulin_offset;
					insulin_offset = 0;
				} else {
					insulin_offset -= ins_dose;
					ins_dose = 0;
				}
			}
			if( ins_dose > 0 ) {
				add_insulin_dose( ts_now, ins_dose , ns::insulin_duration_t::t180 );
			}
		}
		
		if( random_generator( 0, 150 ) < 5 ) {
			auto const carb_dose = static_cast<double>(random_generator( 1, 45 ));
			add_carb_dose( ts_now, carb_dose );
			add_insulin_dose( ts_now, carb_dose/profile.icr );
		}
	
		auto const insulin_drop = mmol_L{ iob_diff * profile.isf };
		auto const carb_rise = mmol_L{(cob_diff / profile.icr) * profile.isf};
		auto const glucose_prev = glucose_state.previous_value( );
		auto const glucose_new = glucose_prev + carb_rise - insulin_drop;
		auto const expected_insulin_drop = mmol_L{ -(profile.isf * iob) };
		auto const expected_carb_rise = mmol_L{ (cob/profile.icr)*profile.isf };
		auto const expected_glucose = glucose_new + expected_insulin_drop + expected_carb_rise;
		glucose_state.add_value( glucose_new );

		std::cout << ts_now << " t=" << cur_duration << " iob=" << iob << " active_ins=" << iob_diff << " cob=" << cob << "g active_carb=" << cob_diff << "g\n";
		std::cout << "\t\tisf*iob=" << expected_insulin_drop << " (cob/icr)*isf=" << expected_carb_rise << " insulin_drop=" << -insulin_drop << " carb_rise=" << carb_rise << '\n';
		std::cout << "\t\tprev_glucose=" << glucose_prev << " glucose=" << glucose_new << " expected_glucose=" << expected_glucose << '\n';
		ts_now += 5min;
		//std::this_thread::sleep_for( 1s );
		clean_up( ts_now, insulin_doses, carb_doses );

		insulin_offset = static_cast<double>(expected_glucose - glucose_state.target_value)/profile.isf;

		// If more insulin is needed to offset higher blood glucose give dose now
		if( insulin_offset > 0.2 ) {
			std::cout << "Bolusing to offset insulin debt need " << insulin_offset << "U\n";
			add_insulin_dose( ts_now, insulin_offset );
			insulin_offset = 0.0;
		}
			
		// Add glucose to blood stream to test reponse
		glucose_state.add_value( glucose_new + glucose_delta );
	}
	return 0;
}

