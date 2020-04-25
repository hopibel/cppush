#include <catch2/catch.hpp>

#include "bool_ops.h"
#include "instruction_manager.h"
#include "instruction.h"

TEST_CASE("InstructionManager.get_by_stack") {
	cppush::InstructionManager im;
	im.register_op(cppush::bool_and, "BOOL.AND", cppush::BOOL);
	im.register_op(cppush::bool_or, "BOOL.OR", cppush::BOOL | cppush::INT);

	auto no_exclude = im.get_by_stack(cppush::BOOL);
	REQUIRE(no_exclude.size() == 2);

	auto exclude_int = im.get_by_stack(cppush::BOOL, cppush::INT);
	REQUIRE(exclude_int.size() == 1);
	REQUIRE(exclude_int[0]->name == "BOOL.AND");

	auto no_match = im.get_by_stack(cppush::INT, cppush::BOOL);
	REQUIRE(no_match.empty());
}
