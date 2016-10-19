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
#include <string>

#include <daw/json/daw_json_link.h>

namespace ns {
	using glucose_t = double;
	using insulin_t = double;

	struct requested_temp_t {
		boost::optional<std::string> error;

	};	// requested_temp

	struct profile_t {
		boost::optional<insulin_t> current_basal;
		boost::optional<insulin_t> max_daily_basal;
		boost::optional<insulin_t> max_basal;
		boost::optional<glucose_t> target_bg;
		boost::optional<glucose_t> min_bg;
		boost::optional<glucose_t> max_bg;
		double sens;
		boost::optional<std::string> model;
		boost::optional<std::string> out_units;
		bool autosens_adjust_targets;
		bool temp_target_set;
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

		iob_data_t( ):
				daw::json::JsonLink<iob_data_t>{ },
				bolussnooze{ },
				activity{ },
				iob{ },
				basaliob{ } {

			link_real( "bolussnooze", bolussnooze );
			link_real( "activity", activity );
			link_real( "iob", iob );				
			link_real( "basaliob", basaliob );
			link_real( "netbasalinsulin", netbasalinsulin );
			link_real( "hightempinsulin", hightempinsulin );
		}
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

