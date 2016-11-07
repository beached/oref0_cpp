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

#include <boost/optional.hpp>
#include <date/date.h>

#include "data_types.h"
#include "lib_iob_calculate.h"

namespace ns {
	namespace lib {
		namespace iob {
			iob_calc_t::iob_calc_t( boost::optional<real_t> iob_contrib, boost::optional<real_t> activity_contrib ):
					iobContrib{ std::move( iob_contrib ) },
					activityContrib{ std::move( activity_contrib ) } { }

			iob_calc_t::iob_calc_t( ):
					iob_calc_t{ boost::none, boost::none } { }

			iob_calc_t::~iob_calc_t( ) { }
		}	// namespace iob
	}	// namespace lib
}    // namespace ns

