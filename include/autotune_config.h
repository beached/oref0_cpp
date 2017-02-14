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

#include <boost/optional.hpp>
#include <string>

#include <daw/json/daw_json_link.h>

namespace ns {
	struct autotune_config_t: public daw::json::JsonLink<autotune_config_t> {
		boost::optional<std::string> api_key;
		std::string nightscout_base_url;

		autotune_config_t( );
		~autotune_config_t( );
		autotune_config_t( autotune_config_t && other );
		autotune_config_t( autotune_config_t const & other );

		autotune_config_t & operator=( autotune_config_t const & ) = default;
		autotune_config_t & operator=( autotune_config_t && ) = default;
	private:
		void link_json( );
	};	// autotune_config_t
}    // namespace ns 

