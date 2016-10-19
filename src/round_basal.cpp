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

#include <boost/algorithm/string/predicate.hpp>
#include <cmath>

#include "data_types.h"
#include "round_basal.h"

namespace ns {
	auto round_basal( double const & basal, profile_t const & profile ) {
		/*
		 * x23 and x54 pumps change basal increment depending on how much basal is being delivered:
		 * 0.025u for 0.025 < x < 0.975
		 * 0.05u for 1 < x < 9.95
		 * 0.1u for 10 < x
		 * To round numbers nicely for the pump, use a scale factor of (1 / increment).
		 */
		auto const lowest_rate_scale = [&profile]( ) -> double { 
			if( profile.model ) {
				auto const & model = *profile.model;
				if( boost::algorithm::ends_with( model, "54" ) || boost::algorithm::ends_with( model, "23" ) ) {
					return 40;
				}
			}
			return 20;
		}( );
		assert( basal > 0 );
		if( basal < 1 ) {
			return round( basal * lowest_rate_scale)/lowest_rate_scale;
		} else if( basal < 10 ) {
			return round( basal * 20.0 )/20.0;
		}
		return round( basal * 10.0 )/10.0;	
	}
}    // namespace ns 

