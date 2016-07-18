cmake_minimum_required(VERSION 3.2)

project(aws CXX)

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
			${CMAKE_SOURCE_DIR}/aws-sdk-cpp
		WORKING_DIRECTORY ${aws-sdk-cpp_DIR}
	)
	execute_process(
		COMMAND ${CMAKE_COMMAND} --build .
		WORKING_DIRECTORY ${aws-sdk-cpp_DIR}
	)
endif()

find_package(aws-sdk-cpp)


add_library(aws.ndll MODULE src/aws.cpp)

set_target_properties(aws.ndll
	PROPERTIES
	PREFIX ""
	OUTPUT_NAME aws
	SUFFIX .ndll
)

target_link_libraries(aws.ndll neko aws-cpp-sdk-s3)