// The MIT License (MIT)
//
// Copyright (c) 2017 Darrell Wright
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

#include <iostream>
#include <vector>

#include <daw/json/daw_json_link.h>

#include "autotune_config.h"
#include "data_types.h"
#include "ns_entries.h"
#include "ns_profiles.h"
#include "ns_treatments.h"

namespace ns {
	using ns_profile_data_t = std::vector<ns::data::profiles::ns_profiles_t>;
	using ns_entries_data_t = std::vector<ns::data::entries::ns_entries_t>;
	using ns_treatments_data_t = std::vector<ns::data::treatments::ns_treatments_t>;

	ns_profile_data_t ns_get_profiles( ns::autotune_config_t const & config );
	ns_entries_data_t ns_get_entries( ns::autotune_config_t const & config, ns::timestamp_t tp_start, ns::timestamp_t tp_end );
	ns_treatments_data_t ns_get_treatments( ns::autotune_config_t const & config, ns::timestamp_t tp_start, ns::timestamp_t tp_end );
}    // namespace ns

std::ostream & operator<<( std::ostream & os, ns::ns_profile_data_t const & profiles );
std::ostream & operator<<( std::ostream & os, ns::ns_entries_data_t const & profiles );
std::ostream & operator<<( std::ostream & os, ns::ns_treatments_data_t const & profiles );

