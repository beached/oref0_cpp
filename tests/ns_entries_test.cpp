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

#include <cstdlib>
#include <iostream>

#include <daw/daw_exception.h>
#include <daw/json/daw_json_link_file.h>

#include "ns_entries.h"

int main( int argc, char **argv ) {
	using nightscout_entries = ns::data::entries::ns_entries_t;
	using namespace ns;
	daw::exception::daw_throw_on_false( argc < 1, "Must supply a filename" );	
	auto entries  = daw::json::array_from_file<nightscout_entries>( argv[1] );
	for( size_t n = 0; n<entries.size( ); ++n ) {
		nightscout_entries const & entry = entries[n];
		auto const str = entry.to_json_string( );
		std::cout << str << '\n';
	}

	return EXIT_SUCCESS;
}
