#include <catch2/catch.hpp>

#include "bool_ops.h"
#include "instruction_manager.h"
#include "instruction.h"
#include "types.h"

//TEST_CASE("InstructionManager.get_by_stack") {
//	cppush::InstructionManager im;
//	im.register_op(cppush::bool_and, "BOOL.AND", cppush::Type::BOOL);
//	im.register_op(cppush::bool_or, "BOOL.OR", cppush::Type::BOOL | cppush::Type::INT);
//
//	auto no_exclude = im.get_by_stack(cppush::Type::BOOL);
//	REQUIRE(no_exclude.size() == 2);
//
//	auto exclude_int = im.get_by_stack(cppush::Type::BOOL, cppush::Type::INT);
//	REQUIRE(exclude_int.size() == 1);
//	REQUIRE(exclude_int[0]->name == "BOOL.AND");
//
//	auto no_match = im.get_by_stack(cppush::Type::INT, cppush::Type::BOOL);
//	REQUIRE(no_match.empty());
//}
