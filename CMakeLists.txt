cmake_minimum_required(VERSION 2.8.12)

project(aws CXX)

include(CTest)


if (APPLE)
	list(LENGTH CMAKE_OSX_ARCHITECTURES OSX_ARCHITECTURES_LEN)
	if (NOT OSX_ARCHITECTURES_LEN EQUAL 1)
		set (CMAKE_OSX_ARCHITECTURES x86_64 CACHE STRING "CMAKE_OSX_ARCHITECTURES should be either i386 or x86_64" FORCE)
	endif()
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


set(AWS_SDK_BUILD_DIR ${CMAKE_BINARY_DIR}/aws-sdk-cpp-build)
set(AWS_SDK_INSTALL_DIR ${AWS_SDK_BUILD_DIR}/prefix)

if (NOT EXISTS ${AWS_SDK_BUILD_DIR})
	file(MAKE_DIRECTORY ${AWS_SDK_BUILD_DIR})
	execute_process(
		COMMAND ${CMAKE_COMMAND}
			-G ${CMAKE_GENERATOR}
			-Wno-dev
			-DBUILD_SHARED_LIBS=OFF
			-DENABLE_UNITY_BUILD=ON
			-DFORCE_SHARED_CRT=ON
			-DENABLE_TESTING=OFF
			"-DBUILD_ONLY=s3;transfer"
			-DCMAKE_INSTALL_PREFIX=${AWS_SDK_INSTALL_DIR}
			${CMAKE_SOURCE_DIR}/aws-sdk-cpp
		WORKING_DIRECTORY ${AWS_SDK_BUILD_DIR}
	)
	execute_process(
		COMMAND ${CMAKE_COMMAND} --build . --target install
		WORKING_DIRECTORY ${AWS_SDK_BUILD_DIR}
	)
endif()

list(APPEND CMAKE_PREFIX_PATH ${AWS_SDK_INSTALL_DIR})
find_package(AWSSDK REQUIRED COMPONENTS s3 transfer)

find_path(NEKO_INCLUDE_DIRS neko.h
	HINTS $ENV{NEKOPATH}/include
)

find_library(NEKO_LIBRARIES NAMES neko
	HINTS $ENV{NEKOPATH}
)

find_program(NEKO_EXECUTABLE neko
	HINTS $ENV{NEKOPATH}
)

find_program(HAXE_EXECUTABLE haxe
	HINTS $ENV{HAXEPATH}
	DOC "Haxe compiler"
)

include_directories(
	${NEKO_INCLUDE_DIRS}
)
add_library(aws.ndll MODULE lib/aws.cpp)

set_target_properties(aws.ndll
	PROPERTIES
	PREFIX ""
	OUTPUT_NAME aws
	SUFFIX .ndll
)

target_link_libraries(aws.ndll ${NEKO_LIBRARIES} aws-cpp-sdk-s3 aws-cpp-sdk-transfer)

file(GLOB_RECURSE haxe_src_files
	src/*.hx
	test/*.hx
)

add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/bin/Test.n
	COMMAND ${HAXE_EXECUTABLE} ${CMAKE_SOURCE_DIR}/test.hxml
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	DEPENDS ${haxe_src_files} ${CMAKE_SOURCE_DIR}/test.hxml
)

add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/bin/aws.ndll
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:aws.ndll> ${CMAKE_SOURCE_DIR}/bin/aws.ndll
	DEPENDS aws.ndll
)

add_custom_target(Test.n ALL
	DEPENDS ${CMAKE_SOURCE_DIR}/bin/Test.n ${CMAKE_SOURCE_DIR}/bin/aws.ndll
)

add_test(
	NAME unittest
	COMMAND ${NEKO_EXECUTABLE} Test.n
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
