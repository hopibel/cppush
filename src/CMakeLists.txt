add_library(cppush
	code.cpp
	code.h
	env.cpp
	env.h
	instruction.h
	instruction_manager.h
	instruction_manager.cpp
	literal.h
	bool_ops.h
	code_ops.h
	common_ops.h
	exec_ops.h
	numeric_ops.h
)
target_include_directories(cppush PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)
