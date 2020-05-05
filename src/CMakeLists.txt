add_library(cppush
	code.cpp
	code.h
	env.cpp
	env.h
	instruction.h
	input_instruction.h
	instruction_set.h
	instruction_set.cpp
	literal.h
	bool_ops.h
	bool_ops.cpp
	code_ops.h
	code_ops.cpp
	common_ops.h
	common_ops.cpp
	exec_ops.h
	exec_ops.cpp
	numeric_ops.h
	numeric_ops.cpp
	types.h
)
target_include_directories(cppush PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)
target_compile_features(cppush PUBLIC cxx_std_17)
