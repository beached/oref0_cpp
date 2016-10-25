cmake_minimum_required( VERSION 2.8.12 )

project( oref0_cpp_prj )

include( ExternalProject )

find_package( Boost 1.58.0 COMPONENTS system iostreams program_options filesystem regex unit_test_framework REQUIRED )

enable_testing( )
add_definitions( -DBOOST_TEST_DYN_LINK ) 

find_package( Threads REQUIRED )

if( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	add_compile_options( -D_WIN32_WINNT=0x0601 ) 
else( )
	if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
		add_compile_options(-std=c++14 -Weverything -pthread -Wno-c++98-compat -g -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion)
	elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
		add_compile_options(-std=c++14 -g -Wall -pthread -Wno-deprecated-declarations)
	endif( )
endif( )

externalproject_add(
    header_libraries_prj
    GIT_REPOSITORY "https://github.com/beached/header_libraries.git"
    SOURCE_DIR "${CMAKE_BINARY_DIR}/dependencies/header_libraries"
    GIT_TAG "master"
    INSTALL_DIR "${CMAKE_BINARY_DIR}/install"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
)

externalproject_add(
    parse_json_prj
    GIT_REPOSITORY "https://github.com/beached/parse_json.git"
    SOURCE_DIR "${CMAKE_BINARY_DIR}/dependencies/parse_json"
    GIT_TAG "master"
    INSTALL_DIR "${CMAKE_BINARY_DIR}/install"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
)

externalproject_add(
    char_range_prj
    GIT_REPOSITORY "https://github.com/beached/char_range.git"
    SOURCE_DIR "${CMAKE_BINARY_DIR}/dependencies/char_range"
    GIT_TAG "master"
    INSTALL_DIR "${CMAKE_BINARY_DIR}/install"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
)

externalproject_add(
    date_prj
    GIT_REPOSITORY "https://github.com/beached/date.git"
    SOURCE_DIR "${CMAKE_BINARY_DIR}/dependencies/date"
    GIT_TAG "master"
    INSTALL_DIR "${CMAKE_BINARY_DIR}/install"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
)

set( HEADER_FOLDER "include" )
set( SOURCE_FOLDER "src" )
set( TEST_FOLDER "tests" )

include_directories( SYSTEM "${CMAKE_BINARY_DIR}/install/include" )
include_directories( ${HEADER_FOLDER} )
include_directories( SYSTEM ${OPENSSL_INCLUDE_DIR} )

include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )

link_directories( "${CMAKE_BINARY_DIR}/install/lib" )
link_directories( ${Boost_LIBRARY_DIRS} )

set( HEADER_FILES
	${HEADER_FOLDER}/data_types.h
	${HEADER_FOLDER}/requested_temp.h
	${HEADER_FOLDER}/round_basal.h
	${HEADER_FOLDER}/lib_iob_calculate.h
	${HEADER_FOLDER}/iob_calc.h
)

set( SOURCE_FILES
	${SOURCE_FOLDER}/data_types.cpp
	${SOURCE_FOLDER}/round_basal.cpp
	${SOURCE_FOLDER}/lib_iob_calculate.cpp
	${SOURCE_FOLDER}/iob_calc.cpp
)

add_library( oref0 ${HEADER_FILES} ${SOURCE_FILES} )
add_dependencies( oref0 header_libraries_prj parse_json_prj char_range_prj date_prj )
target_link_libraries( oref0 parse_json tz char_range ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

add_executable( oref0_get_profile ${HEADER_FILES} ${SOURCE_FOLDER}/bin_oref0_get_profile.cpp )
target_link_libraries( oref0_get_profile oref0 parse_json char_range ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

add_executable( iob_calc_test_bin ${HEADER_FILES} ${SOURCE_FOLDER}/iob_calc_test.cpp )
target_link_libraries( iob_calc_test_bin oref0 parse_json char_range ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

install( TARGETS oref0 DESTINATION lib )
install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/daw/oref0 )
