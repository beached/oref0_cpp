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
#include <date/chrono_io.h>

#include "diabetic_calcs.h"
#include "human.h"
#include "iob_calc.h"
#include "units.h"

using namespace std::chrono_literals;
using namespace std::chrono;
using namespace date;
using namespace ns;

class profile_t {
	isf_t isf;
	icr_t icr;
public:
	profile_t( isf_t ISF, icr_t ICR ): isf{ std::move( ISF ) }, icr{ std::move( ICR ) } { }

	isf_t get_isf( time_point<system_clock> const & ) const {
		return isf;
	}

	icr_t get_icr( time_point<system_clock> const & ) const {
		return icr;
	}

	insulin_rate_t get_basal_rate( time_point<system_clock> const & ) const {
		return 1.2_U_hr;
	}
};	// profile_t

struct glucose_state_t {
	std::vector<glucose_t> values;
	size_t value_capacity;
	glucose_t target_value;

	glucose_t current_value( ) const {
		return values.back( );
	}

	glucose_t previous_value( ) const {
		assert( !values.empty( ) );
		if( values.size( ) == 1 ) {
			return values.front( );
		}
		return *std::prev( values.end( ), 2 );
	}

	void add_value( glucose_t v ) {
		values.push_back( std::move( v ) );
		while( values.size( ) > value_capacity ) {
			values.erase( values.begin( ) );
		}
	}

	glucose_state_t( glucose_t target, size_t capacity = 1 ):
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

constexpr std::chrono::minutes calc_duration( time_point<system_clock> const & lhs, time_point<system_clock> const & rhs ) {
	return duration_cast<minutes>( rhs - lhs);
}

template<typename T>
constexpr T sqr( T value ) {
	value *= value;
	return value;
}

struct carb_dose_t {
	carb_t amount;
	carb_rate_t absorption_rate;
	time_point<system_clock> dose_time;

	carb_dose_t( ) = delete;
	~carb_dose_t( ) = default;
	carb_dose_t( carb_dose_t const & ) = default;
	carb_dose_t & operator=( carb_dose_t const & ) = default;
	carb_dose_t( carb_dose_t && ) = default;
	carb_dose_t & operator=( carb_dose_t && ) = default;

	carb_dose_t( time_point<system_clock> doseTime, carb_t Amount, carb_rate_t absorptionRate ):
			amount{ Amount < 0_g_CHO ? 0_g_CHO : Amount },
			absorption_rate{ absorptionRate < 0_g_CHO_hr ? 0_g_CHO_hr : absorptionRate },
			dose_time{ doseTime } { }

	carb_t tick( time_point<system_clock> const & current_ts ) const {
		auto const carb_duration = amount/absorption_rate;
		std::cout << "carb_duration=" << carb_duration << " absorption_rate=" << absorption_rate << '\n';
		return calculations::calc_cob( amount, carb_duration, calc_duration( dose_time, current_ts ) );
	}
};	// carb_dose_t

void clean_up( time_point<system_clock> const & ts_now, std::vector<insulin_dose> & insulin_doses, std::vector<carb_dose_t> & carb_doses ) {
	insulin_doses.erase( std::remove_if( insulin_doses.begin( ), insulin_doses.end( ), [&]( auto const & item ) {
		auto const duration = calc_duration( item.dose_time, ts_now );
		return duration >= item.dose_dia;
	} ), insulin_doses.end( ) );

	carb_doses.erase( std::remove_if( carb_doses.begin( ), carb_doses.end( ), [&]( auto const & item ) {
		return ts_now > item.dose_time && item.tick( ts_now ) <= 0_g_CHO;
	} ), carb_doses.end( ) );
}

insulin_t calc_iob( time_point<system_clock> const & ts_now, insulin_dose item ) {
	if( ts_now < item.dose_time ) {	// Ignore future insulin
		return item.amount;
	}
	auto const duration = duration_cast<minutes>( ts_now - item.dose_time );
	if( duration > item.dose_dia ) {
		return 0.0_U;
	}
	auto const pc = insulin_on_board_pct( duration, item.dose_dia );
	//std::cout << "~~~~insulin_dose dia=" << static_cast<int16_t>(item.dose_dia) << " age=" << duration << " %=" << pc << " amount=" << item.amount << " left=" << (pc*item.amount) << '\n';
	return item.amount.scale( pc );
}

insulin_t calculate_bolus( time_point<system_clock> const & when, std::chrono::minutes const & dia, profile_t const & profile, carb_t const & carb_dose, carb_rate_t const & carb_absorption_rate ) {
	auto const carb_activity_time = carb_dose / carb_absorption_rate;
	auto const normal_bolus = carb_dose / profile.get_icr( when );
	// Peak carb is currently at carb_activity_time/2.  This may change with a different calc_cob.  Put a find max in if that changes
	auto const peak_carb_activity_time = carb_activity_time / 2;
	daw::exception::daw_throw_on_false( peak_carb_activity_time > 5min );
	
	auto const peak_carb_value = calculations::calc_cob( carb_dose, carb_activity_time, peak_carb_activity_time ) - calculations::calc_cob( carb_dose, carb_activity_time, peak_carb_activity_time - 5min );
	auto const peak_carb_insulin_need = peak_carb_value / profile.get_icr( when );
	auto const total_insulin_need = carb_dose / profile.get_icr( when );

	auto const active_insulin_at_peak_scale = total_insulin_need.scale( insulin_on_board_pct( peak_carb_activity_time, dia ) - insulin_on_board_pct( peak_carb_activity_time - 5min, dia ) );

	auto const available_basal = [&]( ) {
		auto result = 0_U;
		for( auto t = when; t <= when + dia; t += 5min ) {
			insulin_t const current_amount{ real_t{ profile.get_basal_rate( t ).value.value / 12.0 } };
			result += current_amount;		
		}
		return result;
	}( );
	auto const bolus_scale = peak_carb_insulin_need.value / active_insulin_at_peak_scale.value;
	auto proposed_bolus = normal_bolus.scale( bolus_scale );
	std::cout << "Calculating bolus:  proposed_bolus=" << proposed_bolus << " min_bolus=" << normal_bolus << " max_bolus=" << (normal_bolus + available_basal) << '\n';
	if( proposed_bolus < normal_bolus ) {
		return normal_bolus;
	} else if( proposed_bolus > normal_bolus + available_basal ) {
		return normal_bolus + available_basal;
	}
	return proposed_bolus;
}

int main( int, char ** ) {
	//set_default_glucose_display_unit( glucose_unit::mg_dL );
	profile_t profile{ isf_t{ 3.5_mmol_L }, icr_t{ 15_g_CHO } }; 
	auto ts_now = system_clock::now( ); 
	auto const est_liver_carb_rate = calculations::liver_glucose_output( profile.get_basal_rate( ts_now ), profile.get_icr( ts_now ) );
	std::vector<insulin_dose> insulin_doses;
	std::vector<carb_dose_t> carb_doses;
	insulin_t last_iob = 0.0_U;
	carb_t last_cob = 0.0_g_CHO;
	glucose_state_t glucose_state;
	glucose_t glucose_delta = 0.05_mmol_L;
	insulin_t insulin_offset = 0.0_U;

	std::cout << "Based on ICR=" << profile.get_icr( ts_now ) << " ISF=" << profile.get_isf( ts_now ) << " basal_rate=" << profile.get_basal_rate( ts_now );
	std::cout << " it is estimated that the liver outputs " << est_liver_carb_rate << " of glucose or " << est_liver_carb_rate.carbs_per( 1min ) << "/min\n";
	
	auto const add_insulin_dose = [&last_iob, &insulin_doses]( time_point<system_clock> const & when, insulin_t amount, insulin_duration_t dia = insulin_duration_t::t240 ) {
		std::cout << "~~insulin_dose: " << amount << '\n';
		last_iob += amount;
		insulin_doses.emplace_back( amount, dia, when );
	};

	auto const add_carb_dose = [&last_cob, &carb_doses]( time_point<system_clock> const & when, carb_t amount, carb_rate_t absorption_rate = 0.3_g_CHO_min ) {
		std::cout << "~~carb_dose: " << amount << " abs_rate=" << absorption_rate << '\n';
		last_cob += amount;
		carb_doses.emplace_back( when, amount, absorption_rate );
	};

	auto const ts_start = system_clock::now( ); 


//	add_carb_dose( ts_now, 50.0_g_CHO, 0.3_g_CHO_min );
//	add_insulin_dose( ts_now, insulin_offset + calculate_bolus( ts_now, 240min, profile, 50_g_CHO, 0.3_g_CHO_min ) );
	insulin_offset = 0_U;
	simulator_human_t human{ 5.5_mmol_L, 1.2_U_hr };
	auto min_bg = human.get_glucose( );
	auto max_bg = min_bg;

	auto cur_duration = duration_cast<minutes>( ts_now - ts_start );
	auto last_duration = cur_duration;
	while( true ) {
		cur_duration = duration_cast<minutes>( ts_now - ts_start );

		auto const iob = std::accumulate( insulin_doses.begin( ), insulin_doses.end( ), 0.0_U, [&]( auto const & init, auto const & value ) {
			return init + calc_iob( ts_now, value );
		} );

		auto const cob = std::accumulate( carb_doses.begin( ), carb_doses.end( ), 0.0_g_CHO, [&]( auto const & init, auto const & value ) {
			return init + value.tick( ts_now );
		} );

		auto const iob_diff = last_iob - iob; 
		auto const cob_diff = last_cob - cob;
		last_iob = iob;
		last_cob = cob;

		if( cur_duration > last_duration ) {
			auto const carb_dose = est_liver_carb_rate.carbs_per( cur_duration-last_duration );
			last_duration = cur_duration;

			if( cur_duration >= 2h ) {
				add_carb_dose( ts_now, carb_dose, carb_dose/1min );
			}
			auto ins_dose = profile.get_basal_rate( ts_now ).insulin_per( cur_duration-last_duration ); 
			if( insulin_offset < -0.2_U ) {
				std::cout << "Lowering basal to offset too much insulin(TMI): need " << insulin_offset << " and have " << ins_dose << " to give\n";
				if( ins_dose >= insulin_offset ) {
					ins_dose -= insulin_offset;
					insulin_offset = 0.0_U;
				} else {
					insulin_offset -= ins_dose;
					ins_dose = 0.0_U;
				}
			}
			if( ins_dose > 0.0_U ) {
				insulin_offset += ins_dose;
			}
		}
	
/*		if( random_generator( 0, 150 ) < 5 ) {
			auto const carb_dose = carb_t{ static_cast<double>(random_generator( 1, 45 )) };
			add_carb_dose( ts_now, carb_dose );
			insulin_offset += carb_dose/profile.get_icr( ts_now );
		}
*/	
		auto const insulin_drop = iob_diff * profile.get_isf( ts_now );
		auto const carb_rise = (cob_diff / profile.get_icr( ts_now ) ) * profile.get_isf( ts_now );
		auto const glucose_prev = glucose_state.previous_value( );
		auto const glucose_new = glucose_prev + carb_rise - insulin_drop;
		auto const expected_insulin_drop = -(profile.get_isf( ts_now ) * iob);
		auto const expected_carb_rise = (cob/profile.get_icr( ts_now ))*profile.get_isf( ts_now ); 
		auto const expected_glucose = glucose_new + expected_insulin_drop + expected_carb_rise;
		glucose_state.add_value( glucose_new );
		if( glucose_new < min_bg ) {
			min_bg = glucose_new;
		}
		if( glucose_new > max_bg ) {
			max_bg = glucose_new;
		}
		std::cout << " t=" << cur_duration << " iob=" << iob << " active_ins=" << iob_diff << " cob=" << cob << " active_carb=" << cob_diff << "\n";
		std::cout << "\t\tisf*iob=" << expected_insulin_drop << " (cob/icr)*isf=" << expected_carb_rise << " insulin_drop=" << -insulin_drop << " carb_rise=" << carb_rise << '\n';
		std::cout << "\t\tprev_glucose=" << glucose_prev << " glucose=" << glucose_new << " expected_glucose=" << expected_glucose << " min=" << min_bg << " max=" << max_bg << '\n';
		ts_now += 5min;
		//std::this_thread::sleep_for( 1s );
		clean_up( ts_now, insulin_doses, carb_doses );

		insulin_offset += (expected_glucose - glucose_state.target_value)/profile.get_isf( ts_now );

		// If more insulin is needed to offset higher blood glucose give dose now
		if( insulin_offset > 0.02_U ) {
			std::cout << "Bolusing to offset insulin debt need " << insulin_offset << '\n';
			add_insulin_dose( ts_now, insulin_offset );
			insulin_offset = 0.0_U;
		}
			
		// Add glucose to blood stream to test reponse
		glucose_state.add_value( glucose_new + glucose_delta );
	}
	return 0;
}

