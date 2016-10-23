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

#include <array>
#include <boost/optional.hpp>
#include <chrono>
#include <cstdint>
#include <unordered_map>
#include <string>

#include <daw/json/daw_json_link.h>
#include <daw/json/value_t.h>

namespace ns {
	using json_t = daw::json::impl::value_t;
	using json_real = typename json_t::real_t;
	using json_obj_t = typename json_t::object_t;
	using glucose_t = typename json_t::real_t;
	using insulin_t = typename json_t::real_t;
	using index_t = typename json_t::integral_t;
	using timestamp_t = std::chrono::time_point<std::chrono::system_clock>;
	using duration_t = std::chrono::milliseconds;

	struct requested_temp_t {
		boost::optional<std::string> error;

	};	// requested_temp

	enum class profile_types: size_t { current };

	std::ostream & operator<<( std::ostream & os, profile_types t );
	std::istream & operator>>( std::istream & is, profile_types & t );

	struct profile_t: daw::json::JsonLink<profile_t> {
		struct dia_t { };

		bool autosens_adjust_targets;	// true
		bool override_high_target_with_low;	// false
		bool skip_neutral_temps;	// false
		boost::optional<bool> temp_target_set;
		boost::optional<dia_t> dia;
		boost::optional<double> sens;
		boost::optional<glucose_t> max_bg;
		boost::optional<glucose_t> min_bg;
		boost::optional<glucose_t> target_bg;
		boost::optional<insulin_t> current_basal;
		boost::optional<insulin_t> max_basal;
		boost::optional<insulin_t> max_daily_basal;
		boost::optional<std::string> model;
		boost::optional<std::string> out_units;
		double autosens_max;	// 1.2
		double autosens_min;	// 0.7
		double bolussnooze_dia_divider;	// 2.0
		double carbratio_adjustment_ratio;	// 1
		double current_basal_safety_multiplier;	// 4
		double max_dialy_safety_multiplier;	// 3
		double min_5m_carb_impact;	// 3 mg/Dl/5m
		insulin_t max_iob;	// = 0
		profile_types type;	// current

		profile_t( ):
				daw::json::JsonLink<profile_t>{ },
				autosens_adjust_targets{ true },
				override_high_target_with_low{ false },
				skip_neutral_temps{ false },
				temp_target_set{ },
				dia{ },
				sens{ },
				max_bg{ },
				min_bg{ },
				target_bg{ },
				current_basal{ },
				max_basal{ },
				max_daily_basal{ },
				model{ },
				out_units{ },
				autosens_max{ 1.2 },
				autosens_min{ 0.7 },
				bolussnooze_dia_divider{ 2.0 },
				carbratio_adjustment_ratio{ 1.0 },	//if carb ratios on pump are set higher to lower initial bolus using wizard, .8 = assume only 80 percent of carbs covered with full bolus
				current_basal_safety_multiplier{ 4.0 },
				max_dialy_safety_multiplier{ 3.0 },
				min_5m_carb_impact{ 3.0 },	// mg/Dl/5m
				max_iob{ 0 },	// if max_iob is not provided, never give more insulin than the pump would have
				type{ profile_types::current } {

				link_boolean( "autosens_adjust_targets", autosens_adjust_targets );
				link_boolean( "override_high_target_with_low", override_high_target_with_low );
				link_boolean( "skip_neutral_temps", skip_neutral_temps );
				link_boolean( "temp_target_set", temp_target_set );
				//link_object( "dia", dia );
				link_real( "sens", sens );
				link_real( "max_bg", max_bg );
				link_real( "min_bg", min_bg );
				link_real( "target_bg", target_bg );
				link_real( "current_basal", current_basal );
				link_real( "max_basal", max_basal );
				link_real( "max_daily_basal", max_daily_basal );
				link_string( "model", model );
				link_string( "out_units", out_units );
				link_real( "autosens_max", autosens_max  );
				link_real( "autosens_min", autosens_min  );
				link_real( "bolussnooze_dia_divider", bolussnooze_dia_divider  );
				link_real( "carbratio_adjustment_ratio", carbratio_adjustment_ratio  );
				link_real( "current_basal_safety_multiplier", current_basal_safety_multiplier );
				link_real( "max_dialy_safety_multiplier", max_dialy_safety_multiplier  );
				link_real( "min_5m_carb_impact", min_5m_carb_impact  );	// mg/Dl/5m
				link_real( "max_iob", max_iob  );
				link_streamable( "_type", type );
		}
		~profile_t( );

		profile_t( profile_t const & ) = default;
		profile_t( profile_t && ) = default;
		profile_t & operator=( profile_t const & ) = default;
		profile_t & operator=( profile_t && ) = default;

	};	// profile
	
	struct glucose_status_t {
		glucose_t glucose;
		glucose_t delta;
		glucose_t avg_delta;
	};	// glucose_status_t

	struct iob_data_t: public daw::json::JsonLink<iob_data_t> {
		double bolussnooze;
		insulin_t activity;
		insulin_t iob;
		boost::optional<insulin_t> basaliob;
		boost::optional<insulin_t> netbasalinsulin;
		boost::optional<insulin_t> hightempinsulin;

		iob_data_t( double bolusSnooze, 
					insulin_t Activity, 
					insulin_t Iob, 
					boost::optional<insulin_t> basalIob, 
					boost::optional<insulin_t> netBasalInsulin, 
					boost::optional<insulin_t> highTempInsulin );


		iob_data_t( );
		~iob_data_t( );
		
		iob_data_t( iob_data_t const & ) = default;
		iob_data_t( iob_data_t && ) = default;
		iob_data_t & operator=( iob_data_t const & ) = default;
		iob_data_t & operator=( iob_data_t && ) = default;
	};	// iob_data_t

	struct autosense_data_t {
		double ratio;	
	};	// autosense_data_t

	struct meal_data_t {

	};	// meal_data_t

	struct current_temp_t {
		insulin_t rate;
	};	// current_temp_t

	struct determine_basal_exception: public std::runtime_error {
		template<typename... Args>
		determine_basal_exception( Args&&... args ):
				std::runtime_error( std::forward<Args>( args )... ) { }
	};	// determine_basal_exception
}    // namespace ns 

