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

#include <boost/optional.hpp>
#include <boost/utility/string_view.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include <daw/json/daw_json_link.h>

namespace ns {
	namespace impl {
		template<typename Object, typename Member>
		void link_real_duration( boost::string_view json_name, Object obj, boost::optional<Member> & member) {
			using json_real_t = double;
			static auto const to_duration = []( json_real_t const & i ) -> Member {
				return Member{ i };
			};
			static auto const from_duration = []( boost::optional<Member> const & t ) -> boost::optional<json_real_t> {
				if( !t ) {
					return boost::none;
				}
				return t->count( );
			};
			obj->link_json_string_to_real( json_name, member, from_duration, to_duration );
		}

		template<typename Object, typename Duration>
		void link_real_duration( boost::string_view json_name, Object obj, Duration & value) {
			using json_real_t = double;
			static auto const to_duration = []( json_real_t const & i ) -> Duration {
				return Duration{ static_cast<long int>(i) };
			};
			static auto const from_duration = []( Duration const & t ) -> json_real_t {
				return t.count( );
			};
			obj->link_json_string_to_real( json_name, value, from_duration, to_duration );
		}

		template<typename Object, typename Member>
		void link_int_duration( boost::string_view json_name, Object obj, boost::optional<Member> & member) {
			using json_int_t = int64_t;
			static auto const to_duration = []( json_int_t const & i ) -> Member {
				return Member{ static_cast<long int>( i ) };
			};
			static auto const from_duration = []( boost::optional<Member> const & t ) -> boost::optional<json_int_t> {
				if( !t ) {
					return boost::none;
				}
				return t->count( );
			};
			obj->link_jsonintegral( json_name, member, from_duration, to_duration );
		}
		template<typename Object, typename Member>
		void link_int_duration( boost::string_view json_name, Object obj, Member & member) {
			using json_int_t = int64_t;
			static auto const to_duration = []( json_int_t const & i ) -> Member {
				return Member{ i };
			};
			static auto const from_duration = []( Member const & t ) -> json_int_t {
				return t.count( );
			};
			obj->link_jsonintegral( json_name, member, to_duration, from_duration );
		}
	}	// namespace impl
}	// namespace ns

