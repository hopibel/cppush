#include <catch2/catch.hpp>

#include "bool_ops.h"
#include "env.h"
#include "instruction.h"

#include <vector>

TEST_CASE("Instruction: BOOLEAN.AND") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_and, "BOOLEAN.AND");

	bool expected;

	SECTION("F & F = F") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = false;
	}

	SECTION("F & T = F") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = false;
	}

	SECTION("T & T = T") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = true;
	}

	op(env);
	REQUIRE(env.pop<bool>() == expected);
	REQUIRE(env.get_stack<bool>().empty());
}

TEST_CASE("Instruction: BOOLEAN.OR") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_or, "BOOLEAN.OR");

	bool expected;

	SECTION("F | F = F") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = false;
	}

	SECTION("F | T = T") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = true;
	}

	SECTION("T | T = T") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = true;
	}

	op(env);
	REQUIRE(env.pop<bool>() == expected);
	REQUIRE(env.get_stack<bool>().empty());
}

TEST_CASE("Instruction: BOOLEAN.NOT") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_not, "BOOLEAN.NOT");

	bool expected;

	SECTION("!F = T") {
		env.push<bool>(false);
		expected = true;
	}

	SECTION("!T = F") {
		env.push<bool>(true);
		expected = false;
	}

	op(env);
	REQUIRE(env.pop<bool>() == expected);
	REQUIRE(env.get_stack<bool>().empty());
}

TEST_CASE("Instruction: BOOLEAN.NAND") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_nand, "BOOLEAN.NAND");

	bool expected;

	SECTION("F nand F = T") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = true;
	}

	SECTION("F nand T = T") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = true;
	}

	SECTION("T nand T = F") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = false;
	}

	op(env);
	REQUIRE(env.pop<bool>() == expected);
	REQUIRE(env.get_stack<bool>().empty());
}


TEST_CASE("Instruction: BOOLEAN.NOR") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_nor, "BOOLEAN.NOR");

	bool expected;

	SECTION("F nor F = T") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = true;
	}

	SECTION("F nor T = F") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = false;
	}

	SECTION("T nor T = F") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = false;
	}

	op(env);
	REQUIRE(env.pop<bool>() == expected);
	REQUIRE(env.get_stack<bool>().empty());
}

TEST_CASE("Instruction: BOOLEAN.XOR") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_xor, "BOOLEAN.XOR");

	bool expected;

	SECTION("F ^ F = F") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = false;
	}

	SECTION("F ^ T = T") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = true;
	}

	SECTION("T ^ T = F") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = false;
	}

	op(env);
	REQUIRE(env.pop<bool>() == expected);
	REQUIRE(env.get_stack<bool>().empty());
}

TEST_CASE("Instruction: BOOLEAN.INVERT_FIRST_THEN_AND") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_invert_first_then_and, "BOOLEAN.INVERT_FIRST_THEN_AND");

	bool expected;

	SECTION("!F & F = F") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = false;
	}

	SECTION("!F & T = T") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = true;
	}

	SECTION("!T & F = F") {
		env.push<bool>(false);
		env.push<bool>(true);
		expected = false;
	}

	SECTION("!T & T = F") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = false;
	}

	op(env);
	REQUIRE(env.pop<bool>() == expected);
	REQUIRE(env.get_stack<bool>().empty());
}

TEST_CASE("Instruction: BOOLEAN.INVERT_SECOND_THEN_AND") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_invert_second_then_and, "BOOLEAN.INVERT_SECOND_THEN_AND");

	bool expected;

	SECTION("F & !F = F") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = false;
	}

	SECTION("F & !T = F") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = false;
	}

	SECTION("T & !F = T") {
		env.push<bool>(false);
		env.push<bool>(true);
		expected = true;
	}

	SECTION("T & !T = F") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = false;
	}

	op(env);
	REQUIRE(env.pop<bool>() == expected);
	REQUIRE(env.get_stack<bool>().empty());
}
