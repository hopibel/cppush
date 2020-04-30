#include <catch2/catch.hpp>

#include "code_ops.h"
#include "instruction_set.h"
#include "instruction.h"
#include "types.h"

TEST_CASE("InstructionSet.register_core_by_name()") {
	std::vector<cppush::Code_ptr> iset;
	cppush::register_core_by_name(iset, "CODE.NOOP", "CODE.ATOM");
	REQUIRE(iset.size() == 2);
	REQUIRE(*iset[0] == cppush::Instruction(cppush::code_noop, "CODE.NOOP"));
	REQUIRE(*iset[1] == cppush::Instruction(cppush::code_atom, "CODE.ATOM"));
}

//TEST_CASE("InstructionSet.get_by_stack") {
//	cppush::InstructionSet im;
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
