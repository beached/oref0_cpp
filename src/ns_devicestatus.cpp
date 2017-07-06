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

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
#include <string>

#include <daw/json/daw_json_link.h>

#include "ns_devicestatus.h"

namespace ns {
	device_status_pump_battery_item_t::device_status_pump_battery_item_t( ) percent{0} {}

	void device_status_pump_battery_item_t::json_link_map( ) {
		link_json_real( "percent", percent );
	}

	device_status_pump_iob_item_t::device_status_pump_iob_item_t( ) : timestamp{}, bolus_iob{0.0} {}

	void device_status_pump_iob_item_t::json_link_map( ) {
		link_json_timestamp( "timestamp", timestamp );
		link_json_real( "bolusiob", bolus_iob );
	}

	struct device_status_pump_item_t : public daw::json::daw_json_link<device_status_pump_item_t> {
		device_status_pump_battery_item_t battery;
		device_status_pump_iob_item_t iob;
		double reservoir;
		boost::posix_time::ptime clock;

		static void json_link_map( ) {
			link_json_object( "battery", battery );
			link_json_object( "iob", iob );
			link_json_real( "reservoir", reservoir );
			link_json_timestamp( "clock", clock );
		}
	};

	struct device_status_device_item_t : public daw::json::daw_json_link<device_status_device_item_t> {
		std::string id;

		static void json_link_map( ) {
			link_json_string( "id", id );
		}
	};

	struct device_status_uploader_item_t : public daw::json::daw_json_link<device_status_uploader_item_t> {
		uint8_t battery;

		static void json_link_map( ) {
			link_json_integer( "battery", battery );
		}
	};

	struct device_status_sensor_item_t : public daw::json::daw_json_link<device_status_sensor_item_t> {
		uint8_t sensor_age;
		uint8_t sensor_remaining;

		static void json_link_map( ) {
			link_json_integer( "sensor_age", sensor_age );
			link_json_integer( "sensor_remaining", sensor_remaining );
		}
	};

	struct device_status_item_t : public daw::json::daw_json_link<device_status_item_t> {
		std::string id;
		boost::posix_time::ptime created_at;
		boost::optional<device_status_pump_item_t> pump;
		device_status_device_item_t device;
		boost::optional<device_status_uploader_item_t> uploader;
		boost::optional<device_status_sensor_item_t> sensor;
		boost::optional<bool> sensor_not_active;
		boost::optional<device_status_loop_item_t> loop;

		static void json_link_map( ) {
			link_json_string( "id", id );
			link_json_timestamp( "created_at", created_at );
			link_json_object_optional( "pump", pump );
			link_json_object( "device", device );
			link_json_object_optional( "uploader", uploader );
			link_json_object_optional( "sensor", sensor );
			link_json_boolean_optional( "sensor_not_active", sensor_not_active );
			link_json_object_optional( "loop", loop );
		}
	}; // device_status_item_t
} // namespace ns

