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

#include "data_types.h"
#include "units.h"

namespace ns {

	struct human_t {
		virtual ~human_t( );
		virtual void add_carbohydrate( carb_t amount ) = 0;
		virtual void bolus_insulin( insulin_t amount ) = 0;
		virtual void set_basal_rate( insulin_rate_t amount ) = 0;
		virtual insulin_rate_t get_basal_rate( ) const = 0;
		virtual glucose_t get_glucose( ) const = 0;
	};	// human_t

	
	class simulator_human_t: public human_t {
		glucose_t m_current_glucose;
		insulin_rate_t m_current_basal_rate;
	public:
		simulator_human_t( glucose_t current_glucose, insulin_rate_t current_basal_rate );
		~simulator_human_t( );
		simulator_human_t( ) = delete;
		simulator_human_t( simulator_human_t const & ) = default;
		simulator_human_t( simulator_human_t && ) = default;
		simulator_human_t & operator=( simulator_human_t const & ) = default;
		simulator_human_t & operator=( simulator_human_t && ) = default;

		void add_carbohydrate( carb_t amount ) override; 
		void bolus_insulin( insulin_t amount ) override;
		void set_basal_rate( insulin_rate_t amount ) override;
		insulin_rate_t get_basal_rate( ) const override;
		glucose_t get_glucose( ) const override;
	};	// simulator_human_t
}    // namespace ns
