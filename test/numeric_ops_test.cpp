#include <catch2/catch.hpp>

#include "env.h"
#include "numeric_ops.h"

#include <cmath>
#include <vector>

using namespace cppush;

TEMPLATE_TEST_CASE("Instruction: add", "", int, double) {
	Env env;
	Instruction op(add<TestType>, "add");

	env.push<TestType>(2);
	env.push<TestType>(3);
	op(env);
	REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{5});
}

TEMPLATE_TEST_CASE("Instruction: sub", "", int, double) {
	Env env;
	Instruction op(sub<TestType>, "sub");

	env.push<TestType>(2);
	env.push<TestType>(3);
	op(env);
	REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{-1});
}

TEMPLATE_TEST_CASE("Instruction: mult", "", int, double) {
	Env env;
	Instruction op(mul<TestType>, "mult");

	env.push<TestType>(2);
	env.push<TestType>(3);
	op(env);
	REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{6});
}

TEMPLATE_TEST_CASE("Instruction: div", "", int, double) {
	Env env;
	Instruction op(div<TestType>, "div");

	SECTION("Division by zero is noop") {
		env.push<TestType>(1);
		env.push<TestType>(0);
		op(env);
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{1, 0});
	}

	SECTION("2 / 3") {
		env.push<TestType>(2);
		env.push<TestType>(3);
		op(env);
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{
			static_cast<TestType>(2.0/3.0)
		});
	}
}

TEST_CASE("Instruction: integer_mod") {
	Env env;
	Instruction op(int_mod, "integer_mod");

	SECTION("Int modulo 0 is noop") {
		env.push<int>(1);
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{1, 0});
	}

	SECTION("[-2,2] % [-2,2]") {
		for (int i = -2; i <= 2; ++i) {
			for (int j = -2; j <= 2; ++j) {
				if (j == 0) {continue;}
				env.push<int>(i);
				env.push<int>(j);
				op(env);
				REQUIRE(env.get_stack<int>().size() == 1);
				REQUIRE(env.pop<int>() == i % j);
			}
		}
	}
}

TEST_CASE("Instruction: float_mod") {
	Env env;
	Instruction op(float_mod, "float_mod");

	SECTION("Float modulo 0 is noop") {
		env.push<double>(1.0);
		env.push<double>(0.0);
		op(env);
		REQUIRE(env.get_stack<double>() == std::vector<double>{1.0, 0.0});
	}

	SECTION("[-1,1] % [-1,1]") {
		for (double i = -2; i <= 2; i += 0.5) {
			for (double j = -2; j <= 2; j += 0.5) {
				if (j == 0) {continue;}
				env.push<double>(i);
				env.push<double>(j);
				op(env);
				REQUIRE(env.get_stack<double>().size() == 1);
				REQUIRE(env.pop<double>() == std::fmod(i, j));
			}
		}
	}
}

TEMPLATE_TEST_CASE("Instruction: lt", "", int, double) {
	Env env;
	Instruction op(lt<TestType>, "lt");

	for (int i = -2; i <= 2; ++i) {
		for (int j = -2; j <= 2; ++j) {
			env.push<TestType>(i);
			env.push<TestType>(j);
			op(env);
			REQUIRE(env.get_stack<TestType>().empty());
			REQUIRE(env.get_stack<bool>().size() == 1);
			REQUIRE(env.pop<bool>() == (i < j));
		}
	}
}

TEMPLATE_TEST_CASE("Instruction: gt", "", int, double) {
	Env env;
	Instruction op(gt<TestType>, "gt");

	for (int i = -2; i <= 2; ++i) {
		for (int j = -2; j <= 2; ++j) {
			env.push<TestType>(i);
			env.push<TestType>(j);
			op(env);
			REQUIRE(env.get_stack<TestType>().empty());
			REQUIRE(env.get_stack<bool>().size() == 1);
			REQUIRE(env.pop<bool>() == (i > j));
		}
	}
}

TEST_CASE("Instruction: integer_from_boolean") {
	Env env;
	Instruction op(int_from_bool, "integer_from_boolean");

	int expected;
	SECTION("true -> 1") { env.push<bool>(true); expected = 1; }
	SECTION("false -> 0") { env.push<bool>(false); expected = 0; }
	op(env);
	REQUIRE(env.get_stack<bool>().empty());
	REQUIRE(env.get_stack<int>() == std::vector<int>{expected});
}

TEST_CASE("Instruction: integer_from_float") {
	Env env;
	Instruction op(int_from_float, "integer_from_float");

	for (double i = -2.0; i <= 2.0; i += 0.5) {
		env.push<double>(i);
		op(env);
		REQUIRE(env.get_stack<double>().empty());
		REQUIRE(env.get_stack<int>().size() == 1);
		REQUIRE(env.pop<int>() == static_cast<int>(i));
	}
}

TEST_CASE("Instruction: float_from_boolean") {
	Env env;
	Instruction op(float_from_bool, "float_from_boolean");

	double expected;
	SECTION("true -> 1.0") { env.push<bool>(true); expected = 1.0; }
	SECTION("false -> 0.0") { env.push<bool>(true); expected = 1.0; }
	op(env);
	REQUIRE(env.get_stack<bool>().empty());
	REQUIRE( env.get_stack<double>() == std::vector<double>{expected});
}

TEST_CASE("Instruction: float_from_integer") {
	Env env;
	Instruction op(float_from_int, "float_from_integer");

	for (int i = -2; i <= 2; ++i) {
		env.push<int>(i);
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<double>().size() == 1);
		REQUIRE(env.pop<double>() == i);
	}
}

TEST_CASE("Instruction: boolean_from_integer") {
	Env env;
	Instruction op(bool_from_int, "boolean_from_integer");

	for (int i = -2; i <= 2; ++i) {
		env.push<int>(i);
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<bool>().size() == 1);
		REQUIRE(env.pop<bool>() == (i != 0));
	}
}

TEST_CASE("Instruction: boolean_from_float") {
	Env env;
	Instruction op(bool_from_float, "boolean_from_float");

	for (double i = -2; i <= 2; i += 0.5) {
		env.push<double>(i);
		op(env);
		REQUIRE(env.get_stack<double>().empty());
		REQUIRE(env.get_stack<bool>().size() == 1);
		REQUIRE(env.pop<bool>() == (i != 0.0));
	}
}

TEMPLATE_TEST_CASE("Instruction: max", "", int, double) {
	Env env;
	Instruction op(max<TestType>, "max");

	for (TestType i = -2; i <= 2; ++i) {
		for (TestType j = -2; j <= 2; ++j) {
			env.push<TestType>(i);
			env.push<TestType>(j);
			op(env);
			REQUIRE(env.get_stack<TestType>().size() == 1);
			REQUIRE(env.pop<TestType>() == std::max(i, j));
		}
	}
}

TEMPLATE_TEST_CASE("Instruction: min", "", int, double) {
	Env env;
	Instruction op(min<TestType>, "min");

	for (TestType i = -2; i <= 2; ++i) {
		for (TestType j = -2; j <= 2; ++j) {
			env.push<TestType>(i);
			env.push<TestType>(j);
			op(env);
			REQUIRE(env.get_stack<TestType>().size() == 1);
			REQUIRE(env.pop<TestType>() == std::min(i, j));
		}
	}
}

TEST_CASE("Instruction: float_cos") {
	Env env;
	Instruction op(trig_cos, "float_cos");

	for (double i = -2; i <= 2; i += 0.1) {
		env.push<double>(i);
		op(env);
		REQUIRE(env.get_stack<double>().size() == 1);
		REQUIRE(env.pop<double>() == std::cos(i));
	}
}

TEST_CASE("Instruction: float_sin") {
	Env env;
	Instruction op(trig_sin, "float_sin");

	for (double i = -2; i <= 2; i += 0.1) {
		env.push<double>(i);
		op(env);
		REQUIRE(env.get_stack<double>().size() == 1);
		REQUIRE(env.pop<double>() == std::sin(i));
	}
}

TEST_CASE("Instruction: float_tan") {
	Env env;
	Instruction op(trig_tan, "float_tan");

	for (double i = -2; i <= 2; i += 0.1) {
		env.push<double>(i);
		op(env);
		REQUIRE(env.get_stack<double>().size() == 1);
		REQUIRE(env.pop<double>() == std::tan(i));
	}
}
