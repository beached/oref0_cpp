// The MIT License (MIT)
//
// Copyright (c) 2016-2017 Darrell Wright
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
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>

#include <daw/json/JsonLink.h>

#include "ns_devicestatus.h"

namespace ns {
	device_status_pump_battery_item_t::device_status_pump_battery_item_t( ): 
			daw::json::JsonLink<device_status_pump_battery_item_t>{ },
			percent{ 0 } {

		link_number( "percent", percent );
	}

	device_status_pump_battery_item_t::device_status_pump_battery_item_t( device_status_pump_battery_item_t const & other ):
			daw::json::JsonLink<device_status_pump_battery_item_t>{ },
			percent{ other.percent } {

		link_number( "percent", percent );
	}
				
	device_status_pump_iob_item_t::device_status_pump_iob_item_t( ):
			daw::json::JsonLink<device_status_pump_iob_item_t>{ },
			timestamp{ },
			bolus_iob{ 0.0 } {
	
		link_timestamp( "timestamp", timestamp );
		link_real( "bolusiob", bolus_iob );
	}

	device_status_pump_iob_item_t::device_status_pump_iob_item_t( device_status_pump_iob_item_t const & other ):
			daw::json::JsonLink<device_status_pump_iob_item_t>{ },
			timestamp{ other.timestamp },
			bolus_iob{ other.bolus_iob } {
	
		link_timestamp( "timestamp", timestamp );
		link_real( "bolusiob", bolus_iob );
	}

	struct device_status_pump_item_t: public daw::json::JsonLink<device_status_pump_item_t> {
		device_status_pump_battery_item_t battery;
		device_status_pump_iob_item_t iob;
		double reservoir;
		boost::posix_time::ptime clock;

		device_status_pump_item_t( );
		device_status_pump_item_t( device_status_pump_item_t const & );
		device_status_pump_item_t & operator=( device_status_pump_item_t const & );
	};

	struct device_status_device_item_t: public daw::json::JsonLink<device_status_device_item_t> {
		std::string id;

		device_status_device_item_t( );
		device_status_device_item_t( device_status_device_item_t const & );
		device_status_device_item_t & operator=( device_status_device_item_t const & );
	};

	struct device_status_uploader_item_t: public daw::json::JsonLink<device_status_uploader_item_t> {
		uint8_t battery;

		device_status_uploader_item_t( );
		device_status_uploader_item_t( device_status_uploader_item_t const & );
		device_status_uploader_item_t & operator=( device_status_uploader_item_t const & );
	};

	struct device_status_sensor_item_t: public daw::json::JsonLink<device_status_sensor_item_t> {
		uint8_t sensor_age;
		uint8_t sensor_remaining;

		device_status_sensor_item_t( );
		device_status_sensor_item_t( device_status_sensor_item_t const & );
		device_status_sensor_item_t & operator=( device_status_sensor_item_t const & );
	};

	struct device_status_item_t: public daw::json::JsonLink<device_status_item_t> {
		std::string id;
		boost::posix_time::ptime created_at;
		boost::optional<device_status_pump_item_t> pump;
		device_status_device_item_t device;
		boost::optional<device_status_uploader_item_t> uploader;
		boost::optional<device_status_sensor_item_t> sensor;
		boost::optional<bool> sensor_not_active;
		boost::optional<device_status_loop_item_t> loop;

		device_status_item_t( );
		device_status_item_t( device_status_item_t const & );
		device_status_item_t & operator=( device_status_item_t const & );
	};	// device_status_item_t
}    // namespace ns 

