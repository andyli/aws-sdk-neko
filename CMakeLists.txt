cmake_minimum_required(VERSION 3.2)

project(aws CXX)

include(CTest)


if (APPLE)
	if ("${CMAKE_OSX_ARCHITECTURES}" STREQUAL "i386")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ndll/Mac)
	elseif ("${CMAKE_OSX_ARCHITECTURES}" STREQUAL "x86_64")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ndll/Mac64)
	else()
		message(FATAL_ERROR "CMAKE_OSX_ARCHITECTURES should be either i386 or x86_64")
	endif()
elseif(UNIX)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ndll/Linux)
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ndll/Linux64)
	else()
		message(FATAL_ERROR "unknown CMAKE_SIZEOF_VOID_P: ${CMAKE_SIZEOF_VOID_P}")
	endif()
elseif(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ndll/Windows)
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ndll/Windows64)
	else()
		message(FATAL_ERROR "unknown CMAKE_SIZEOF_VOID_P: ${CMAKE_SIZEOF_VOID_P}")
	endif()
else()
	message(FATAL_ERROR "unknown platform")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# avoid the extra "Debug", "Release" directories
# http://stackoverflow.com/questions/7747857/in-cmake-how-do-i-work-around-the-debug-and-release-directories-visual-studio-2
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
	string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )


set(aws-sdk-cpp_DIR ${CMAKE_BINARY_DIR}/aws-sdk-cpp-build)

if (NOT EXISTS ${aws-sdk-cpp_DIR})
	file(MAKE_DIRECTORY ${aws-sdk-cpp_DIR})
	execute_process(
		COMMAND ${CMAKE_COMMAND}
			-Wno-dev
			-DBUILD_SHARED_LIBS=OFF
			-DENABLE_UNITY_BUILD=ON
			-DFORCE_SHARED_CRT=OFF
			-DENABLE_TESTING=OFF
			"-DBUILD_ONLY=s3;transfer"
			${CMAKE_SOURCE_DIR}/aws-sdk-cpp
		WORKING_DIRECTORY ${aws-sdk-cpp_DIR}
	)
	execute_process(
		COMMAND ${CMAKE_COMMAND} --build .
		WORKING_DIRECTORY ${aws-sdk-cpp_DIR}
	)
endif()

find_package(aws-sdk-cpp)


include_directories(
	/usr/local/include
)
link_directories(
	/usr/local/lib
)
add_library(aws.ndll MODULE lib/aws.cpp)

set_target_properties(aws.ndll
	PROPERTIES
	PREFIX ""
	OUTPUT_NAME aws
	SUFFIX .ndll
)

target_link_libraries(aws.ndll neko aws-cpp-sdk-s3 aws-cpp-sdk-transfer)

set(HAXE haxe CACHE STRING "Haxe compiler")
set(NEKO neko CACHE STRING "Neko VM")

add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/bin/Test.n
	COMMAND ${HAXE} ${CMAKE_SOURCE_DIR}/test.hxml
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/bin/aws.ndll
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:aws.ndll> ${CMAKE_SOURCE_DIR}/bin/aws.ndll
)

add_custom_target(Test.n ALL
	DEPENDS ${CMAKE_SOURCE_DIR}/bin/Test.n ${CMAKE_SOURCE_DIR}/bin/aws.ndll
)

add_test(
	NAME unittest
	COMMAND ${NEKO} Test.n
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
