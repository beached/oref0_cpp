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
#include <boost/lexical_cast.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

struct parameters {
	std::vector<std::string> ordered_parameters;
	std::map<std::string, std::string> optional_parameters;

	template<typename T>
	T ordered_as( size_t n ) const {
		return boost::lexical_cast<T>( ordered_parameters.at( n ) );
	}
};

parameters parse_cmd_line( int argc, char** argv ) {
	parameters result;
	for( int n=1; n<argc; ++n ) {
		if( boost::starts_with( argv[n], "--" ) ) {

		} else {
			result.ordered_parameters.emplace_back( argv[n] );
		}
	}
	return result;
}

int main( int argc, char** argv ) {
	namespace po = boost::program_options;
	po::positional_options_description pos_desc;
	std::vector<std::pair<std::string, bool>> const positional_parameters = {
			{ "pump_settings", false },
			{ "bg_targets", false },
			{ "insulin_sensitivities", false },
			{ "basal_profile", false },
			{ "preferences", true },
			{ "carb_ratios", true },
			{ "temp_targets", true }
	};
	for( auto const & param : positional_parameters ) {
		pos_desc.add( param.first.c_str( ), 1 );
	}

	po::options_description desc{ "Options" };
	desc.add_options( )
			( "help", "Print option descriptions" )
			( "model", po::value<std::string>( ), "Pump model" );

	po::variables_map vm;
	try {
		po::store( po::command_line_parser( argc, argv ).options( desc ).positional( pos_desc ).run( ), vm );

		if( vm.count( "help" ) ) {
			std::cout << argv[0];
			for( auto const & param: positional_parameters ) {
				auto const bracket_chars = param.second ? std::make_pair<char>( '[', ']' ) : std::make_pair<char>( '<', '>' );
				std::cout << ' ' << bracket_chars.first << param.first << ".json" << bracket_chars.second;
			}
			std::cout << '\n' << desc << std::endl;
			return EXIT_SUCCESS;
		}
		po::notify( vm );
	} catch( po::error const & po_error ) {
		std::cerr << "ERROR: " << po_error.what( ) << '\n';
		std::cerr << desc << std::endl;
		return EXIT_FAILURE;
	}



	return EXIT_SUCCESS;
}
