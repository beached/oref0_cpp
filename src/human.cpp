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

#include "data_types.h"
#include "units.h"

#include "human.h"

namespace ns {
	simulator_human_t::simulator_human_t( glucose_t current_glucose, insulin_rate_t current_basal_rate ):
			m_current_glucose{ std::move( current_glucose ) },
			m_current_basal_rate{ std::move( current_basal_rate ) } { }

	human_t::~human_t( ) { }

	simulator_human_t::~simulator_human_t( ) { }

	void simulator_human_t::add_carbohydrate( carb_t amount ) { }

	void simulator_human_t::bolus_insulin( insulin_t amount ) { }

	void simulator_human_t::set_basal_rate( insulin_rate_t amount ) { }

	insulin_rate_t simulator_human_t::get_basal_rate( ) const {
		return m_current_basal_rate;
	}

	glucose_t simulator_human_t::get_glucose( ) const {
		return m_current_glucose;
	}

}    // namespace ns 

