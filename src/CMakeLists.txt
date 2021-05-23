add_library(cppush
	code.cpp
	number_ops.cpp
	pushgp.cpp
	state.cpp
)

target_include_directories(cppush
	PUBLIC ../include
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(cppush PUBLIC cxx_std_17)

target_compile_options(cppush PRIVATE -Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wfatal-errors)
