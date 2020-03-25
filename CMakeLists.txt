cmake_minimum_required(VERSION 3.5)
project(cppush LANGUAGES CXX)

include(CTest)

add_subdirectory(src)
add_subdirectory(test)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(cppush_test PRIVATE -Wall -Wextra)
endif()
