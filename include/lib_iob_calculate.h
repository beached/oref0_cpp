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
#include <date/date.h>

#include "data_types.h"

namespace ns {
	namespace lib {
		namespace iob {
			struct iob_calc_t {
				boost::optional<insulin_t> iobContrib;
				boost::optional<insulin_t> activityContrib;
				
				iob_calc_t( boost::optional<insulin_t> iob_contrib, boost::optional<insulin_t> activity_contrib );
				iob_calc_t( );
				~iob_calc_t( );
				iob_calc_t( iob_calc_t const & ) = default;
				iob_calc_t( iob_calc_t && ) = default;
				iob_calc_t & operator=( iob_calc_t const & ) = default;
				iob_calc_t & operator=( iob_calc_t && ) = default;

			};	// iob_calc_t

			template<typename treatment_t, typename dia_t>
			iob_calc_t iobCalc( treatment_t const & treatment, dia_t const & dia ) {
				auto const diaratio = 3.0 / dia;
				auto const peak = 75;
				auto const end = 180;

				if( treatment.insulin ) {
					using namespace std::chrono;
					auto const minAgo = diaratio * duration_cast<minutes>(system_clock::now( ) - treatment.date).count( );

					/*
					 * these magic numbers are the result of an excel curve fit to find the insulin activity curve that would result in activity rising linearly 
					 * from 0 to a peak at 75m and then back to 0 again at DIA. The 75m was for either 3h or 4h DIA, and then that is scaled together with DIA when 
					 * that changes.
					 */
					if( minAgo < peak ) {
						auto const x = minAgo/5.0 + 1.0;
						insulin_t iobContrib = treatment.insulin * (1.0 - 0.001852 * sqr( x ) + 0.001852 * x);
						insulin_t activityContrib = treatment.insulin * (2.0 / dia / 60.0 / peak ) * minAgo;

						return { {std::move( iobContrib )}, {std::move( activityContrib )} };
					} else if( minAgo < end ) {
						auto const y = (minAgo-peak)/5.0;
						insulin_t iobContrib = treatment.insulin  * (0.001323 * sqr( y ) - 0.054233 * y + 0.55556);
						insulin_t activityContrib = treatment.insulin * (2.0 / dia / 60.0 - (minAgo - peak) * 2.0 / dia / 60.0 / (60.0 * dia - peak)); 

						return { {std::move( iobContrib )}, {std::move( activityContrib )} };
					}
				}

				return { };
			}
		}	// namespace iob
	}	// namespace lib
}    // namespace ns

