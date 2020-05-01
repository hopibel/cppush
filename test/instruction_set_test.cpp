#include <catch2/catch.hpp>

#include "code_ops.h"
#include "instruction_set.h"
#include "instruction.h"
#include "types.h"

#include <vector>

TEST_CASE("register_core_by_name()") {
	std::vector<cppush::Instruction_ptr> iset;
	cppush::register_core_by_name(iset, "code_noop");
	REQUIRE(iset.size() == 1);
	REQUIRE(iset[0]->name == "code_noop");
}

TEST_CASE("register_core_by_stack()") {
	std::vector<cppush::Instruction_ptr> iset;
	cppush::register_core_by_stack(iset, cppush::Type::Bool);
	REQUIRE(iset.size() == 15); // 14 Bool-only + code_noop
}
