cmake_minimum_required( VERSION 2.8.12 )

project( oref0_cpp_prj )

include( ExternalProject )

find_package( Boost 1.58.0 COMPONENTS system date_time iostreams program_options filesystem regex unit_test_framework REQUIRED )
find_package( CURL REQUIRED )

enable_testing( )
add_definitions( -DBOOST_TEST_DYN_LINK ) 

find_package( Threads REQUIRED )

if( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	add_compile_options( -D_WIN32_WINNT=0x0601 /std:c++latest ) 
else( )
	if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
		add_compile_options(-std=c++14 -Weverything -pthread -Wno-c++98-compat -g -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion -Wno-c99-compat)
	elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
		add_compile_options(-std=c++14 -ggdb -Wall -pthread -Wno-deprecated-declarations -march=native )
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
include_directories( SYSTEM ${CURL_INCLUDE_DIRS} )

include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )

link_directories( "${CMAKE_BINARY_DIR}/install/lib" )
link_directories( ${Boost_LIBRARY_DIRS} )

set( HEADER_FILES
	${HEADER_FOLDER}/autotune.h
	${HEADER_FOLDER}/carb_time_unit.h
	${HEADER_FOLDER}/carb_unit.h
	${HEADER_FOLDER}/data_types.h
	${HEADER_FOLDER}/diabetic_calcs.h
	${HEADER_FOLDER}/dbl_error.h
	${HEADER_FOLDER}/glucose_unit.h
	${HEADER_FOLDER}/human.h
	${HEADER_FOLDER}/icr_unit.h
	${HEADER_FOLDER}/insulin_time_unit.h
	${HEADER_FOLDER}/insulin_unit.h
	${HEADER_FOLDER}/iob_calc.h
	${HEADER_FOLDER}/isf_unit.h
	${HEADER_FOLDER}/lib_iob_calculate.h
	${HEADER_FOLDER}/ns_entries.h
	${HEADER_FOLDER}/requested_temp.h
	${HEADER_FOLDER}/iob_calc.h
	${HEADER_FOLDER}/units.h
)

set( SOURCE_FILES
	${SOURCE_FOLDER}/autotune.cpp
	${SOURCE_FOLDER}/carb_time_unit.cpp
	${SOURCE_FOLDER}/carb_unit.cpp
	${SOURCE_FOLDER}/data_types.cpp
	${SOURCE_FOLDER}/diabetic_calcs.cpp
	${SOURCE_FOLDER}/dbl_error.cpp
	${SOURCE_FOLDER}/glucose_unit.cpp
	${SOURCE_FOLDER}/human.cpp
	${SOURCE_FOLDER}/icr_unit.cpp
	${SOURCE_FOLDER}/insulin_time_unit.cpp
	${SOURCE_FOLDER}/insulin_unit.cpp
	${SOURCE_FOLDER}/iob_calc.cpp
	${SOURCE_FOLDER}/isf_unit.cpp
	${SOURCE_FOLDER}/lib_iob_calculate.cpp
	${SOURCE_FOLDER}/ns_entries.cpp
	${SOURCE_FOLDER}/units.cpp
)

add_library( oref0 ${HEADER_FILES} ${SOURCE_FILES} )
add_dependencies( oref0 header_libraries_prj parse_json_prj char_range_prj date_prj )
target_link_libraries( oref0 parse_json tz char_range ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

add_executable( simulator_bin ${HEADER_FILES} ${SOURCE_FOLDER}/simulator.cpp )
target_link_libraries( simulator_bin oref0 parse_json char_range ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

add_executable( iob_calc_test_bin ${HEADER_FILES} ${TEST_FOLDER}/iob_calc_test.cpp )
target_link_libraries( iob_calc_test_bin oref0 parse_json char_range ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

add_executable( cob_calc_test_bin ${HEADER_FILES} ${TEST_FOLDER}/cob_calc_test.cpp )
target_link_libraries( cob_calc_test_bin oref0 parse_json char_range ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

add_executable( glucose_unit_test_bin ${HEADER_FILES} ${TEST_FOLDER}/glucose_unit_test.cpp )
target_link_libraries( glucose_unit_test_bin oref0 parse_json char_range ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

add_executable( ns_entries_test_bin ${HEADER_FILES} ${TEST_FOLDER}/ns_entries_test.cpp )
target_link_libraries( ns_entries_test_bin oref0 parse_json char_range ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

add_executable( autotune_tune_bin ${HEADER_FILES} ${TEST_FOLDER}/autotune_test.cpp )
target_link_libraries( autotune_tune_bin oref0 parse_json char_range ${Boost_LIBRARIES} ${CURL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

install( TARGETS oref0 DESTINATION lib )
install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/daw/oref0 )
