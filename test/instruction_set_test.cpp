#include <catch2/catch.hpp>

#include "code_ops.h"
#include "instruction_set.h"
#include "instruction.h"
#include "types.h"

#include <vector>

TEST_CASE("register_core_by_name()") {
	std::vector<cppush::Instruction_ptr> iset;
	cppush::register_core_by_name(iset, "CODE.NOOP");
	REQUIRE(iset.size() == 1);
	REQUIRE(iset[0]->name == "CODE.NOOP");
}

TEST_CASE("register_core_by_stack()") {
	std::vector<cppush::Instruction_ptr> iset;
	cppush::register_core_by_stack(iset, cppush::Type::BOOL);
	REQUIRE(iset.size() == 15); // 14 BOOL-only + CODE.NOOP
}
