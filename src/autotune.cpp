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

#include <chrono>
#include <future>
#include <date/date.h>

#include <daw/curl_wrapper.h>

#include "autotune.h"

namespace ns {
	namespace impl {
		namespace {
			void sort_data( ns_profile_data_t & profile_data, ns_entries_data_t & glucose_data, ns_treatments_data_t & treatments_data ) {
				std::sort( std::begin( profile_data ), std::end( profile_data ), []( auto const & lhs, auto const & rhs ) {
					return lhs.start_date < rhs.start_date;
				} );
				std::sort( std::begin( glucose_data ), std::end( glucose_data ), []( auto const & lhs, auto const & rhs ) {
					return lhs.timestamp < rhs.timestamp;
				} );
				std::sort( std::begin( treatments_data ), std::end( treatments_data ), []( auto const & lhs, auto const & rhs ) {
					return lhs.timestamp < rhs.timestamp;
				} );
			}

			auto autotune_data( ns_profile_data_t profile_data, ns_entries_data_t glucose_data, ns_treatments_data_t treatments_data ) {
				sort_data( profile_data, glucose_data, treatments_data );
				/*
				auto current_profile = profile_data.begin( );
				for( ; current_profile != profile_data.end( ); ++current_profile ) {
					auto next_profile = std::next( current_profile );
					if( next_profile != profile_data.end( ) ) {
						if( next_profile.start_date > )
					}
				}
				 */
				return ns::data::profiles::ns_profiles_t{ };
			}
		}    // namespace anonymous
	}	// namespace impl

	ns::data::profiles::ns_profiles_t autotune_data( boost::string_view nightscout_base_url, std::chrono::system_clock::time_point const tp_start, std::chrono::system_clock::time_point const tp_end ) {
		auto const launch_policy = std::launch::deferred;
		/*
		auto profile_data = std::async( launch_policy, ns::ns_get_profiles, nightscout_base_url );
		auto glucose_data = std::async( launch_policy, ns::ns_get_entries, nightscout_base_url, tp_start, tp_end );
		auto treatments_data = std::async( launch_policy, ns::ns_get_treatments, nightscout_base_url, tp_start, tp_end );
		return impl::autotune_data( profile_data.get( ), glucose_data.get( ), treatments_data.get( ) );
		*/
		auto profile_data = ns::ns_get_profiles( nightscout_base_url );
		auto glucose_data = ns::ns_get_entries( nightscout_base_url, tp_start, tp_end );
		auto treatments_data = ns::ns_get_treatments( nightscout_base_url, tp_start, tp_end );
		return impl::autotune_data( profile_data, glucose_data, treatments_data );
	}
}	// namespace ns

