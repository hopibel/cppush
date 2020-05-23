#include <catch2/catch.hpp>

#include "code.h"
#include "code_ops.h"
#include "env.h"
#include "instruction_set.h"
#include "types.h"

#include <vector>

using namespace cppush;

TEST_CASE("register_core_by_name()") {
	auto iset = register_core_by_name({"code_noop"});
	REQUIRE(iset == std::vector<Instruction>{Instruction(code_noop, "code_noop")});
}

TEST_CASE("register_core_by_stack()") {
	auto iset = register_core_by_stack(Types(0));
	REQUIRE(iset == std::vector<Instruction>{Instruction(code_noop, "code_noop")});
}

TEST_CASE("register_n_inputs()") {
	auto iset = register_n_inputs<2>();
	Env env;
	env.load_program(
		Program{CodeList()},
		{ 1, 2.0 }
	);
	iset[0](env);
	iset[1](env);
	REQUIRE(env.get_stack<int>() == std::vector<int>{1});
	REQUIRE(env.get_stack<double>() == std::vector<double>{2.0});
}
