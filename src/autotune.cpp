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

#include "autotune.h"

namespace ns {

	ns_profile_data_t get_nightscout_profile_data( boost::string_view nightscout_base_url ) {

		return ns_profile_data_t { };
	}

	ns_glucose_data_t get_nightscout_glucose_data( boost::string_view nightscout_base_url ) {

		return ns_glucose_data_t { };
	}

	ns_treatment_data_t get_nightscout_treatment_data( boost::string_view nightscout_base_url ) {

		return ns_treatment_data_t { };
	}

	ns_profile_data_t autotune_data( ns_profile_data_t const & profile_data, ns_glucose_data_t const & glucose_data, ns_treatment_data_t const & treatment_data ) {

		return ns_profile_data_t { };
	}
}	// namespace ns

