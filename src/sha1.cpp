// The MIT License ( MIT )
//
// Copyright ( c ) 2017 Darrell Wright
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

#include <array>
#include <boost/uuid/sha1.hpp>
#include <cstdint>
#include <iomanip>
#include <sstream>

#include <daw/daw_string_view.h>

#include "sha1.h"

namespace daw {
	namespace {
		class sha1_t {
			boost::uuids::detail::sha1 m_sha1;

		  public:
			sha1_t( daw::string_view msg ) : m_sha1{} {
				process( msg );
			}

			sha1_t &process( daw::string_view msg ) {
				m_sha1.process_bytes( msg.data( ), msg.size( ) );
				return *this;
			}

			std::string to_string( ) {
				unsigned int digest[5];
				m_sha1.get_digest( digest );

				std::ostringstream buf;
				for( auto const &item : digest ) {
					buf << std::hex << std::setfill( '0' ) << std::setw( 8 ) << item;
				}
				auto result = buf.str( );
				return result;
			}
		}; // sha1
	}      // namespace

	std::string sha1( daw::string_view message ) noexcept {
		return sha1_t{message}.to_string( );
	}
} // namespace daw

