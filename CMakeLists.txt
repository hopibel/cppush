cmake_minimum_required(VERSION 3.5)
project(cppush LANGUAGES CXX)

include(CTest)

add_subdirectory(src)
add_subdirectory(test)

target_compile_features(cppush_test PUBLIC cxx_std_17)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(cppush_test PRIVATE -Wall -Wextra)
endif()
