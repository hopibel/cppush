#include <catch2/catch.hpp>

#include "bool_ops.h"
#include "env.h"
#include "instruction.h"

#include <vector>

TEST_CASE("Instruction: BOOLEAN.AND") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_and, "BOOLEAN.AND");

	SECTION("F & F = F") {
		env.bool_stack = std::vector<bool>{false, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}

	SECTION("F & T = F") {
		env.bool_stack = std::vector<bool>{true, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}

	SECTION("T & F = F") {
		env.bool_stack = std::vector<bool>{false, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}

	SECTION("T & T = T") {
		env.bool_stack = std::vector<bool>{true, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}
}

TEST_CASE("Instruction: BOOLEAN.OR") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_or, "BOOLEAN.OR");

	SECTION("F | F = F") {
		env.bool_stack = std::vector<bool>{false, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}

	SECTION("F | T = T") {
		env.bool_stack = std::vector<bool>{true, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("T | F = T") {
		env.bool_stack = std::vector<bool>{false, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("T | T = T") {
		env.bool_stack = std::vector<bool>{true, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}
}

TEST_CASE("Instruction: BOOLEAN.NOT") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_not, "BOOLEAN.NOT");

	SECTION("!F = T") {
		env.bool_stack = std::vector<bool>{false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("!T = F") {
		env.bool_stack = std::vector<bool>{true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}
}

TEST_CASE("Instruction: BOOLEAN.NAND") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_nand, "BOOLEAN.NAND");

	SECTION("F nand F = T") {
		env.bool_stack = std::vector<bool>{false, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("F nand T = T") {
		env.bool_stack = std::vector<bool>{true, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("T nand F = T") {
		env.bool_stack = std::vector<bool>{false, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("T nand T = F") {
		env.bool_stack = std::vector<bool>{true, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}
}


TEST_CASE("Instruction: BOOLEAN.NOR") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_nor, "BOOLEAN.NOR");

	SECTION("F nor F = T") {
		env.bool_stack = std::vector<bool>{false, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("F nor T = F") {
		env.bool_stack = std::vector<bool>{true, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}

	SECTION("T nor F = F") {
		env.bool_stack = std::vector<bool>{false, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}

	SECTION("T nor T = F") {
		env.bool_stack = std::vector<bool>{true, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}
}

TEST_CASE("Instruction: BOOLEAN.XOR") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_xor, "BOOLEAN.XOR");

	SECTION("F ^ F = F") {
		env.bool_stack = std::vector<bool>{false, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}

	SECTION("F ^ T = T") {
		env.bool_stack = std::vector<bool>{true, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("T ^ F = T") {
		env.bool_stack = std::vector<bool>{false, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("T ^ T = F") {
		env.bool_stack = std::vector<bool>{true, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}
}

TEST_CASE("Instruction: BOOLEAN.INVERT_FIRST_THEN_AND") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_invert_first_then_and, "BOOLEAN.INVERT_FIRST_THEN_AND");

	SECTION("!F & F = F") {
		env.bool_stack = std::vector<bool>{false, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}

	SECTION("!F & T = T") {
		env.bool_stack = std::vector<bool>{true, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("!T & T = F") {
		env.bool_stack = std::vector<bool>{false, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}

	SECTION("!T & T = F") {
		env.bool_stack = std::vector<bool>{true, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}
}

TEST_CASE("Instruction: BOOLEAN.INVERT_SECOND_THEN_AND") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_invert_second_then_and, "BOOLEAN.INVERT_SECOND_THEN_AND");

	SECTION("F & !F = F") {
		env.bool_stack = std::vector<bool>{false, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}

	SECTION("F & !T = F") {
		env.bool_stack = std::vector<bool>{true, false};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}

	SECTION("T & !F = T") {
		env.bool_stack = std::vector<bool>{false, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("T & !T = F") {
		env.bool_stack = std::vector<bool>{true, true};
		op(env);
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}
}
