#include <catch2/catch.hpp>

#include "code_ops.h"
#include "instruction.h"
#include "input_instruction.h"
#include "instruction_set.h"
#include "types.h"

#include <vector>

TEST_CASE("register_core_by_name()") {
	std::vector<cppush::Instruction> iset;
	cppush::register_core_by_name(iset, {"code_noop"});
	REQUIRE(iset.size() == 1);
	REQUIRE(iset[0] == cppush::Instruction(cppush::code_noop, {"code_noop"}));
}

TEST_CASE("register_core_by_stack()") {
	std::vector<cppush::Instruction> iset;
	cppush::register_core_by_stack(iset, cppush::Types(0));
	REQUIRE(iset.size() == 1); // code_noop
	REQUIRE(iset[0] == cppush::Instruction(cppush::code_noop, {"code_noop"}));
}

//TEST_CASE("register_n_inputs()") {
//	std::vector<cppush::Code_ptr> iset;
//	cppush::register_n_inputs(iset, 3);
//
//	std::vector<int> inputs{0, 1, 2};
//	cppush::Env env;
//	env.load_inputs(inputs);
//
//	for (int i = 0; i < 3; ++i) {
//		(*iset[i])(env);
//		REQUIRE(env.pop<int>() == inputs[i]);
//	}
//}
