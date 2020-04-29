find_package(Catch2 REQUIRED)

add_executable(cppush_test
	test_main.cpp
	bool_ops_test.cpp
	code_ops_test.cpp
	common_ops_test.cpp
	exec_ops_test.cpp
	numeric_ops_test.cpp

	instruction_set_test.cpp
)
target_link_libraries(cppush_test cppush Catch2::Catch2)

include(Catch)
catch_discover_tests(cppush_test)
