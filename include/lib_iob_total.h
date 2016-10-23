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

#include <chrono>

#include "data_types.h"

namespace ns {
	auto iob_total( object_t const & opts ) {
		auto now = std::chrono::system_clock::now( );
		auto iob_calc = opts( "calculate" );
		if( !opts.has_member( "treatments" ) ) {
			return { };
		}

		auto treatments = [&]( ) -> typename json_t::array_t {
			auto result = opts( "treatments" );
			if( !result.is_array( ) ) {
				throw std::runtime_error( "Expected an array" );
			}
			return result.get_array( );
		}( );

		auto profile_data = opts( "profile" );
		insulin_t iob = 0;
		double bolussnooze = 0;
		insulin_t basaliob = 0;
		insulin_t activity = 0;
		insulin_t netbasalinsulin = 0;
		insulin_t hightempinsulin = 0;

		for( auto const & treatment: treatments ) {
			


		}
	}
}    // namespace ns

