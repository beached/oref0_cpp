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
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <boost/utility/string_view.hpp>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include <daw/daw_bounded_stack.h>

#include "data_types.h"

std::pair<std::string, std::string> parse_kv_string( boost::string_view line ) {
	daw::bounded_stack_t<char, 2> ch_stack;
	bool in_quote = false;
	for( size_t pos=0; pos<line.size( ); ++pos ) {
		switch( line[pos] ) {
			case '\\':
				ch_stack.push_back( '\\' );
			break;
			case '\"': {
				ch_stack.push_back( '\"' );
				break;
			}
			case '=': 
				if( !in_quote ) {
					auto sep = std::next( line.begin( ), static_cast<std::iterator_traits<decltype(line.begin( ))>::difference_type>(pos) );
					return { boost::trim_copy( std::string{ line.begin( ), sep } ), boost::trim_copy( std::string{ std::next( sep ), line.end( ) } ) };
				}
				break;
		}
		if( !ch_stack.empty( ) ) {
			if( ch_stack.front( ) == '\\' ) {
				if( ch_stack.used( ) == 2 ) {
					ch_stack.clear( );
				}
			} else if( ch_stack.front( ) == '\"' ) {
				in_quote = !in_quote;
				ch_stack.clear( );
			}
		}
	}
	return { boost::trim_copy( std::string{ line.begin( ), line.end( ) } ), "" };
}

struct parameters {
	std::vector<std::string> ordered_parameters;
	std::unordered_map<std::string, std::string> optional_parameters;

	template<typename T>
	T ordered_as( size_t n ) const {
		return boost::lexical_cast<T>( ordered_parameters.at( n ) );
	}

	boost::optional<std::string> kv_get( boost::string_view key ) const {
		auto it = optional_parameters.find( key.to_string( ) );
		if( it != optional_parameters.end( ) ) {
			return { it->second };
		}
		return { };
	}

	size_t ordered_count( ) const {
		return ordered_parameters.size( );
	}
};

parameters parse_cmd_line( int const argc, char const * const * argv ) {
	parameters result;
	for( int n=1; n<argc; ++n ) {
		if( boost::starts_with( argv[n], "--" ) ) {
			result.optional_parameters.insert( parse_kv_string( argv[n] + 2 ) );
		} else {
			result.ordered_parameters.emplace_back( argv[n] );
		}
	}
	return result;
}

struct unnamed_parameters {
	std::vector<std::string> required;
	std::vector<std::string> optional;

	unnamed_parameters( std::vector<std::string> Required, std::vector<std::string> Optional ):
			required{ std::move( Required ) },
			optional{ std::move( Optional ) } { }

	boost::optional<std::string> find( parameters const & param, std::string const & key ) const {
		{
			auto const pos = std::find( required.begin( ), required.end( ), key );
			if( pos != required.end( ) ) {
				auto const dist = static_cast<size_t>(std::distance( required.begin( ), pos ));
				return { param.ordered_parameters[dist] };
			}
		}
		auto const pos = std::find( optional.begin( ), optional.end( ), key );
		if( pos == optional.end( ) ) {
			return { };
		}
		auto const dist = static_cast<size_t>(std::distance( optional.begin( ), pos )) + required.size( );
		return { param.ordered_parameters[dist] };
	}
};

void show_help( char const * name, unnamed_parameters const & unnamed ) {
	std::cout << name;
	for( auto const & param: unnamed.required ) {
		std::cout << " <" << param << ".json>";
	}
	for( auto const & param: unnamed.optional ) {
		std::cout << " [" << param << ".json]";
	}
	std::cout << "\nOptional Parameters\n--------------------\n";
	std::cout << "--help - Display this help message\n";
	std::cout << "--model=<pump model> - Model of pump\n";
	std::cout << "--exportDefaults\n";
	std::cout << "--updatePreferences=<true|false>" << std::endl;
}

int main( int argc, char** argv ) {
	auto const params = parse_cmd_line( argc, argv );
	unnamed_parameters const unnamed{ { "pump_settings", "bg_targets", "insulin_sensitivities", "basal_profile" }, { "preferencces", "carb_ratios", "temp_targets" } };

	if( params.kv_get( "help" ) ) {
		show_help( argv[0], unnamed );
		return EXIT_SUCCESS;
	}
	auto const model = params.kv_get( "model" );	

	if( params.kv_get( "exportDefaults" ) ) {
		std::cout << (ns::profile_t{ }).to_string( ) << std::endl;
		return EXIT_SUCCESS;
	} else if( params.kv_get( "updatePreferences" ) ) {
		auto const prefs_file_name = params.kv_get( "updatePreferences" ); 
		if( prefs_file_name && !prefs_file_name->empty( ) ) {
			boost::filesystem::path const pref_file{ *prefs_file_name };
			if( exists( pref_file ) && is_regular_file( pref_file ) ) {
				try {
					auto prefs = daw::json::from_file<ns::profile_t>( pref_file.native( ) );	
					std::cout << prefs.to_string( ) << std::endl;
					return EXIT_SUCCESS;
				} catch( std::exception const & ex ) {
					std::cerr << "Error importing prefernces\n" << ex.what( ) << std::endl;
					exit( EXIT_FAILURE );
				}
			}
		}
		std::cout << (ns::profile_t{ }).to_string( ) << std::endl;
		return EXIT_SUCCESS;
	}


	if( params.ordered_count( ) < unnamed.required.size( ) || params.ordered_count( ) > (unnamed.required.size( ) + unnamed.optional.size( )) ) {
		show_help( argv[0], unnamed );
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}
