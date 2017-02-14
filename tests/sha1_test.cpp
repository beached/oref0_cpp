// The MIT License ( MIT )
//
// Copyright ( c ) 2016-2017 Darrell Wright
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

#define BOOST_TEST_MODULE sha1_test
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "sha1.h"

#define TEST1 "abc"
#define TEST2a  "abcdbcdecdefdefgefghfghighijhi"
#define TEST2b "jkijkljklmklmnlmnomnopnopq"
#define TEST2 TEST2a TEST2b
#define TEST3 "a"
#define TEST4a "01234567012345670123456701234567"
#define TEST4b "01234567012345670123456701234567"

/* an exact multiple of 512 bits */
#define TEST4 TEST4a TEST4b
namespace {
	char const * testarray[4] = {
		TEST1,
		TEST2,
		TEST3,
		TEST4
	};

	long int repeatcount[4] = { 1, 1, 1000000, 10 };

	char const * resultarray[4] = {
		"A9 99 3E 36 47 06 81 6A BA 3E 25 71 78 50 C2 6C 9C D0 D8 9D",
		"84 98 3E 44 1C 3B D2 6E BA AE 4A A1 F9 51 29 E5 E5 46 70 F1",
		"34 AA 97 3C D4 C4 DA A4 F6 1E EB 2B DB AD 27 31 65 34 01 6F",
		"DE A3 56 A2 CD DD 90 C7 A7 EC ED C5 EB B5 63 93 4F 46 04 52"
	};
}	// namespace anonymous

BOOST_AUTO_TEST_CASE( sha1_001 ) {
	SHA1Context sha;
	int i, j, err;
	uint8_t Message_Digest[20];

	/*
	 *	Perform SHA-1 tests
	 *
	 */
	for( j = 0; j < 4; ++j ) {
		std::cout << "\nTest " << (j+1) << ": " << repeatcount[j] << ", '" << testarray[j] << "'\n";

		SHA1Reset( sha );

		for( i = 0; i < repeatcount[j]; ++i ) {
			err = SHA1Input( sha, ( unsigned char const * ) testarray[j], strlen( testarray[j] ) );
			if( err ) {
				std::cerr << "SHA1Input Error " << err << ".\n";
				break;    /* out of for i loop */
			}
		}

		err = SHA1Result( sha, Message_Digest );
		if( err ) {
			std::cerr << "SHA1Result Error " << err << ", could not compute message digest.\n";
		} else {
			std::cout << '\t';
			for( i = 0; i < 20 ; ++i ) {
				std::cout << std::hex << Message_Digest[i];
			}
			std::cout << '\n';
		}
		std::cout << "Should match:\n\t" << resultarray[j] << '\n';
	}
}
