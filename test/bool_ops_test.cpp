#include <catch2/catch.hpp>

#include "bool_ops.h"
#include "env.h"

#include <vector>

using namespace cppush;

TEST_CASE("Instruction: boolean_and") {
	Env env;
	Instruction op(bool_and, "boolean_and");
	bool expected;

	SECTION("F F -> F") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = false;
	}
	SECTION("F T -> F") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = false;
	}
	SECTION("T T -> T") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = true;
	}
	op(env);
	REQUIRE(env.get_stack<bool>() == std::vector<bool>{expected});
}

TEST_CASE("Instruction: boolean_or") {
	Env env;
	Instruction op(bool_or, "boolean_or");
	bool expected;

	SECTION("F F -> F") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = false;
	}
	SECTION("F T -> T") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = true;
	}
	SECTION("T T -> T") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = true;
	}
	op(env);
	REQUIRE(env.get_stack<bool>() == std::vector<bool>{expected});
}

TEST_CASE("Instruction: boolean_not") {
	Env env;
	Instruction op(bool_not, "boolean_not");
	bool expected;

	SECTION("F -> T") {
		env.push<bool>(false);
		expected = true;
	}
	SECTION("T -> F") {
		env.push<bool>(true);
		expected = false;
	}
	op(env);
	REQUIRE(env.get_stack<bool>() == std::vector<bool>{expected});
}

TEST_CASE("Instruction: boolean_nand") {
	Env env;
	Instruction op(bool_nand, "boolean_nand");
	bool expected;

	SECTION("F F -> T") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = true;
	}
	SECTION("F T -> T") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = true;
	}
	SECTION("T T -> F") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = false;
	}
	op(env);
	REQUIRE(env.get_stack<bool>() == std::vector<bool>{expected});
}

TEST_CASE("Instruction: boolean_nor") {
	Env env;
	Instruction op(bool_nor, "boolean_nor");
	bool expected;

	SECTION("F F -> T") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = true;
	}
	SECTION("F T -> F") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = false;
	}
	SECTION("T T -> F") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = false;
	}
	op(env);
	REQUIRE(env.get_stack<bool>() == std::vector<bool>{expected});
}

TEST_CASE("Instruction: boolean_xor") {
	Env env;
	Instruction op(bool_xor, "boolean_xor");
	bool expected;

	SECTION("F F -> F") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = false;
	}
	SECTION("F T -> T") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = true;
	}
	SECTION("T T -> F") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = false;
	}
	op(env);
	REQUIRE(env.get_stack<bool>() == std::vector<bool>{expected});
}

TEST_CASE("Instruction: boolean_invert_first_then_and") {
	Env env;
	Instruction op(bool_invert_first_then_and, "boolean_invert_first_then_and");
	bool expected;

	SECTION("F F -> F") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = false;
	}
	SECTION("F T -> T") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = true;
	}
	SECTION("T F -> F") {
		env.push<bool>(false);
		env.push<bool>(true);
		expected = false;
	}
	SECTION("T T -> F") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = false;
	}
	op(env);
	REQUIRE(env.get_stack<bool>() == std::vector<bool>{expected});
}

TEST_CASE("Instruction: boolean_invert_second_then_and") {
	Env env;
	Instruction op(bool_invert_second_then_and, "boolean_invert_second_then_and");
	bool expected;

	SECTION("F F -> F") {
		env.push<bool>(false);
		env.push<bool>(false);
		expected = false;
	}
	SECTION("F T -> F") {
		env.push<bool>(true);
		env.push<bool>(false);
		expected = false;
	}
	SECTION("T F -> T") {
		env.push<bool>(false);
		env.push<bool>(true);
		expected = true;
	}
	SECTION("T T -> F") {
		env.push<bool>(true);
		env.push<bool>(true);
		expected = false;
	}
	op(env);
	REQUIRE(env.get_stack<bool>() == std::vector<bool>{expected});
}
