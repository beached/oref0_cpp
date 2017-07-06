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

#include "data_types.h"

namespace ns {
	profile_t::~profile_t( ) { }

	std::ostream & operator<<( std::ostream & os, profile_types t ) {
		using namespace std::literals::string_literals;
		static std::array<std::string, 1> const results = { {"current"s} };
		os << results[static_cast<size_t>(t)];
		return os;
	}

	std::istream & operator>>( std::istream & is, profile_types & t ) {
		using namespace std::literals::string_literals;
		static std::unordered_map<std::string, profile_types> const results = { {"current"s, profile_types::current} };
		std::string s;
		is >> s;
		t = results.at( s );
		return is;
	}

	iob_data_t::iob_data_t( double bolusSnooze, insulin_t Activity, insulin_t Iob, boost::optional<insulin_t> basalIob, boost::optional<insulin_t> netBasalInsulin, boost::optional<insulin_t> highTempInsulin ):
			daw::json::daw_json_link<iob_data_t>{ },
			bolussnooze{ std::move( bolusSnooze ) },
			activity{ std::move( Activity ) },
			iob{ std::move( Iob ) },
			basaliob{ std::move( basalIob ) },
			hightempinsulin{ std::move( highTempInsulin ) } {

		/*
		link_real( "bolussnooze", bolussnooze );
		link_real( "activity", activity );
		link_real( "iob", iob );				
		link_real( "basaliob", basaliob );
		link_real( "netbasalinsulin", netbasalinsulin );
		link_real( "hightempinsulin", hightempinsulin );
		*/
	}

	iob_data_t::iob_data_t( ):
			iob_data_t{ { }, { }, { }, boost::none, boost::none, boost::none } { }

	iob_data_t::~iob_data_t( ) { }

}    // namespace ns 
