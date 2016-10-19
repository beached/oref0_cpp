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

#include <boost/optional.hpp>
#include <cmath>
#include <string>

#include "data_types.h"

namespace ns {
	template<typename T, typename U>
	constexpr auto round_to( T const & value, double digits ) {
		auto const scale = pow( 10, digits );
		return round( value * scale )/scale;
	}

	std::string to_fixed( double d, int8_t digits ) {
		assert( digits >= 0 );
		std::stringstream ss;
		ss << std::fixed << std::setprecision( digits ) << d;
		return ss.str( );
	}

	/// @brief we expect BG to rise or fall at the rate of BGI,
	/// adjusted by the rate at which BG would need to rise /
	/// fall to get eventualBG to target over DIA/2 hours
	template<typename T> 
	constexpr auto calculate_expected_delta( T const & dia, glucose_t const & target_bg, glucose_t const & eventual_bg, glucose_t const & bgi ) {
		// (hours*mins_per_hour)/5 = how many 5 minute periods in a dia/2
		auto const dia_in_5min_blocks = (dia/2 * 60)/5;
		auto const target_delta = target_bg - eventual_bg;
		auto expected_delta = round_to( bgi + (target_delta / dia_in_5min_blocks), 1 );
		return expected_delta;
	}

	constexpr auto convert_bg( glucose_t const & value, profile_t const & profile ) {
		if( profile.out_units == "mmol/L" ) {
			return to_fixed( round_to( static_cast<double>(value) * 0.0555, 1 ), 1 );
		}
		return to_fixed( value, 0 );
	}


	template<typename F>
	auto determine_basal( glucose_status_t const & glucose_status, current_temp_t const & current_temp, iob_data_t & iob_data, profile_t const & profile, boost::optional<autosense_data_t> const & autosense_data, meal_data_t const & meal_data, F temp_basal_functions ) {
		if( !profile.current_basal ) { 	
			throw determine_basal_exception( "Could not get current basal rate" );
		}

		auto const basal = [&]( ) = {
			auto result = *profile.current_basal;
			if( autosense_data ) {
				result *= autosense_data->ratio;
				result = round( basal * 100.0 )/100.0;
				if( result != *profile.current_basal ) {
					std::clog << "Adjusting basal from " << *(profile.current_basal) << " to " << basal << '\n';
				}
			}
			return result;
		}( );

		auto const bg = glucose_status.glucose;
		// TODO: figure out how to use raw isig data to estimate BG
		if( bg < 30 ) {	
			std::stringstream reason;
			reason << "CGM is calibrating or in ??? state";
			if( basal <= current_temp.rate ) {
				reason << "; setting current basal of " << basal << " as temp";
				return temp_basal_functions.set_temp_basal( basal, 30, profile, reason, current_temp );
			}
			reason << ", temp " << current_temp.rate << " <~ current basal " << basal << "U/hr";
			throw determine_basal_exception( reason.str( ) );
		}

		// if target_bg is set, great. otherwise, if min and max are set, then set target to their average

		if( !profile.min_bg || !profile.max_bg ) {
			throw determine_basal_exception( "Missing min/max glucose numbers" );
		}
		auto min_bg = *profile.min_bg;
		auto max_bg = *profile.max_bg;

		glucose_t const target_bg = [&]( ) {
			if( profile.target_bg ) {
				return *profile.target_bg;
			} else {
				return (min_bg + max_bg)/2;
			} 
		}( );

		
		// adjust min, max, and target BG for sensitivity, such that 50% increase in ISF raises target from 100 to 120
		if( autosens_data && profile.autosens_adjust_targets ) {
			if( profile.temp_target_set ) {
				std::clog << "Temp target set, not adjusting with autosens\n";
			} else {
				auto const & ratio = autosens_data->ratio; 
				min_bg = round( (min_bg - 60)/ratio ) + 60;
				min_bg = round( (max_bg - 60)/ratio ) + 60;
				auto new_target_bg = round( (target_bg - 60)/ratio ) + 60;
				if( target_bg == new_target_bg ) {
					std::clog << "target_bg unchanged: " << target_bg << '\n';
				} else {
					std::clog << "Adjusting target_bg from " << target_bg << " to " << new_target_bg << '\n';
				}
				target_bg = new_target_bg;
			}
		}

		auto iob_array = iob_data;

	}
}    // namespace ns 
