#include <catch2/catch.hpp>

#include "env.h"
#include "instruction.h"
#include "numeric_ops.h"

#include <cmath>
#include <vector>

TEMPLATE_TEST_CASE("Instruction: ADD", "", int, double) {
	cppush::Env env;
	auto& stack = env.get_stack<TestType>();
	cppush::Instruction op(cppush::add<TestType>, "+");

	for (int i = -2; i <= 2; ++i) {
		for (int j = -2; j <= 2; ++j) {
			stack.clear();
			stack.push_back(i);
			stack.push_back(j);
			op(env);
			REQUIRE(stack.size() == 1);
			REQUIRE(stack.back() == i + j);
		}
	}
}

TEMPLATE_TEST_CASE("Instruction: SUB", "", int, double) {
	cppush::Env env;
	auto& stack = env.get_stack<TestType>();
	cppush::Instruction op(cppush::sub<TestType>, "-");

	for (int i = -2; i <= 2; ++i) {
		for (int j = -2; j <= 2; ++j) {
			stack.clear();
			stack.push_back(i);
			stack.push_back(j);
			op(env);
			REQUIRE(stack.size() == 1);
			REQUIRE(stack.back() == i - j);
		}
	}
}

TEMPLATE_TEST_CASE("Instruction: MUL", "", int, double) {
	cppush::Env env;
	auto& stack = env.get_stack<TestType>();
	cppush::Instruction op(cppush::mul<TestType>, "*");

	for (int i = -2; i <= 2; ++i) {
		for (int j = -2; j <= 2; ++j) {
			stack.clear();
			stack.push_back(i);
			stack.push_back(j);
			op(env);
			REQUIRE(stack.size() == 1);
			REQUIRE(stack.back() == i * j);
		}
	}
}

TEMPLATE_TEST_CASE("Instruction: DIV", "", int, double) {
	cppush::Env env;
	auto& stack = env.get_stack<TestType>();
	cppush::Instruction op(cppush::div<TestType>, "/");

	SECTION("Division by zero is noop") {
		stack.push_back(1);
		stack.push_back(0);
		op(env);
		REQUIRE(stack.size() == 2);
		REQUIRE(stack[0] == 1);
		REQUIRE(stack[1] == 0);
	}

	SECTION("[-2,2] / [-2,2]") {
		for (int i = -2; i <= 2; ++i) {
			for (int j = -2; j <= 2; ++j) {
				if (j == 0) {continue;}
				stack.clear();
				stack.push_back(i);
				stack.push_back(j);
				op(env);
				REQUIRE(stack.size() == 1);
				REQUIRE(stack.back() == TestType(i) / TestType(j));
			}
		}
	}
}

TEST_CASE("Instruction: INT.%") {
	cppush::Env env;
	cppush::Instruction op(cppush::int_mod, "INT.%");

	SECTION("Int modulo 0 is noop") {
		env.int_stack.push_back(1);
		env.int_stack.push_back(0);
		op(env);
		REQUIRE(env.int_stack.size() == 2);
		REQUIRE(env.int_stack[0] == 1);
		REQUIRE(env.int_stack[1] == 0);
	}

	SECTION("[-2,2] % [-2,2]") {
		for (int i = -2; i <= 2; ++i) {
			for (int j = -2; j <= 2; ++j) {
				if (j == 0) {continue;}
				env.int_stack.clear();
				env.int_stack.push_back(i);
				env.int_stack.push_back(j);
				op(env);
				REQUIRE(env.int_stack.size() == 1);
				REQUIRE(env.int_stack.back() == i % j);
			}
		}
	}
}

TEST_CASE("Instruction: FLOAT.%") {
	cppush::Env env;
	cppush::Instruction op(cppush::float_mod, "FLOAT.%");

	SECTION("Float modulo 0 is noop") {
		env.float_stack.push_back(1.0);
		env.float_stack.push_back(0.0);
		op(env);
		REQUIRE(env.float_stack.size() == 2);
		REQUIRE(env.float_stack[0] == 1.0);
		REQUIRE(env.float_stack[1] == 0.0);
	}

	SECTION("[-1,1] % [-1,1]") {
		for (double i = -2; i <= 2; i += 0.1) {
			for (double j = -2; j <= 2; j += 0.1) {
				if (j == 0) {continue;}
				env.float_stack.clear();
				env.float_stack.push_back(i);
				env.float_stack.push_back(j);
				op(env);
				REQUIRE(env.float_stack.size() == 1);
				REQUIRE(env.float_stack.back() == std::fmod(i, j));
			}
		}
	}
}

TEMPLATE_TEST_CASE("Instruction: LT", "", int, double) {
	cppush::Env env;
	auto& stack = env.get_stack<TestType>();
	auto& bool_stack = env.get_stack<bool>();
	cppush::Instruction op(cppush::less_than<TestType>, "<");

	for (int i = -2; i <= 2; ++i) {
		for (int j = -2; j <= 2; ++j) {
			stack.clear();
			bool_stack.clear();
			stack.push_back(i);
			stack.push_back(j);
			op(env);
			REQUIRE(stack.empty());
			REQUIRE(bool_stack == std::vector<bool>{i < j});
		}
	}
}

TEMPLATE_TEST_CASE("Instruction: GT", "", int, double) {
	cppush::Env env;
	auto& stack = env.get_stack<TestType>();
	auto& bool_stack = env.get_stack<bool>();
	cppush::Instruction op(cppush::greater_than<TestType>, ">");

	for (int i = -2; i <= 2; ++i) {
		for (int j = -2; j <= 2; ++j) {
			stack.clear();
			bool_stack.clear();
			stack.push_back(i);
			stack.push_back(j);
			op(env);
			REQUIRE(stack.empty());
			REQUIRE(bool_stack == std::vector<bool>{i > j});
		}
	}
}

TEST_CASE("Instruction: INT.FROMBOOL") {
	cppush::Env env;
	cppush::Instruction op(cppush::int_from_bool, "INT.FROMBOOL");

	env.bool_stack.push_back(true);
	op(env);
	REQUIRE(env.bool_stack.empty());
	REQUIRE(env.int_stack == std::vector<int>{1});

	env.bool_stack.clear();
	env.int_stack.clear();

	env.bool_stack.push_back(false);
	op(env);
	REQUIRE(env.bool_stack.empty());
	REQUIRE(env.int_stack == std::vector<int>{0});
}

TEST_CASE("Instruction: INT.FROMFLOAT") {
	cppush::Env env;
	cppush::Instruction op(cppush::int_from_float, "INT.FROMFLOAT");

	for (double i = -2.0; i <= 2.0; i += 0.5) {
		env.int_stack.clear();
		env.float_stack.clear();
		env.float_stack.push_back(i);
		op(env);
		REQUIRE(env.float_stack.empty());
		REQUIRE(env.int_stack == std::vector<int>{static_cast<int>(i)});
	}
}

TEST_CASE("Instruction: FLOAT.FROMBOOL") {
	cppush::Env env;
	cppush::Instruction op(cppush::float_from_bool, "FLOAT.FROMBOOL");

	SECTION("true -> 1.0") {
		env.bool_stack.push_back(true);
		op(env);
		REQUIRE(env.bool_stack.empty());
		REQUIRE(env.float_stack == std::vector<double>{1.0});
	}

	SECTION("false -> 0.0") {
		env.bool_stack.push_back(false);
		op(env);
		REQUIRE(env.bool_stack.empty());
		REQUIRE(env.float_stack == std::vector<double>{0.0});
	}
}

TEST_CASE("Instruction: FLOAT.FROMINT") {
	cppush::Env env;
	cppush::Instruction op(cppush::float_from_int, "FLOAT.FROMINT");

	for (int i = -2; i <= 2; ++i) {
		env.float_stack.clear();
		env.int_stack.clear();
		env.int_stack.push_back(i);
		op(env);
		REQUIRE(env.int_stack.empty());
		REQUIRE(env.float_stack == std::vector<double>{static_cast<double>(i)});
	}
}

TEST_CASE("Instruction: BOOLEAN.FROMINT") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_from_int, "BOOLEAN.FROMINT");

	for (int i = -2; i <= 2; ++i) {
		env.bool_stack.clear();
		env.int_stack.clear();
		env.int_stack.push_back(i);
		op(env);
		REQUIRE(env.int_stack.empty());
		REQUIRE(env.bool_stack == std::vector<bool>{i != 0});
	}
}

TEST_CASE("Instruction: BOOLEAN.FROMFLOAT") {
	cppush::Env env;
	cppush::Instruction op(cppush::bool_from_float, "BOOLEAN.FROMFLOAT");

	for (double i = -2; i <= 2; i += 0.5) {
		env.bool_stack.clear();
		env.float_stack.clear();
		env.float_stack.push_back(i);
		op(env);
		REQUIRE(env.float_stack.empty());
		REQUIRE(env.bool_stack == std::vector<bool>{i != 0.0});
	}
}

TEMPLATE_TEST_CASE("Instruction: MAX", "", int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::max<TestType>, "MAX");
	auto& stack = env.get_stack<TestType>();

	for (TestType i = -2; i <= 2; ++i) {
		for (TestType j = -2; j <= 2; ++j) {
			stack.clear();
			stack.push_back(i);
			stack.push_back(j);
			op(env);
			REQUIRE(stack == std::vector<TestType>{i > j ? i : j});
		}
	}
}

TEMPLATE_TEST_CASE("Instruction: MIN", "", int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::min<TestType>, "MIN");
	auto& stack = env.get_stack<TestType>();

	for (TestType i = -2; i <= 2; ++i) {
		for (TestType j = -2; j <= 2; ++j) {
			stack.clear();
			stack.push_back(i);
			stack.push_back(j);
			op(env);
			REQUIRE(stack == std::vector<TestType>{i < j ? i : j});
		}
	}
}

TEST_CASE("Instruction: FLOAT.COS") {
	cppush::Env env;
	cppush::Instruction op(cppush::trig_cos, "FLOAT.COS");

	for (double i = -2; i <= 2; i += 0.1) {
		env.float_stack.clear();
		env.float_stack.push_back(i);
		op(env);
		REQUIRE(env.float_stack == std::vector<double>{std::cos(i)});
	}
}

TEST_CASE("Instruction: FLOAT.SIN") {
	cppush::Env env;
	cppush::Instruction op(cppush::trig_sin, "FLOAT.SIN");

	for (double i = -2; i <= 2; i += 0.1) {
		env.float_stack.clear();
		env.float_stack.push_back(i);
		op(env);
		REQUIRE(env.float_stack == std::vector<double>{std::sin(i)});
	}
}

TEST_CASE("Instruction: FLOAT.TAN") {
	cppush::Env env;
	cppush::Instruction op(cppush::trig_tan, "FLOAT.TAN");

	for (double i = -2; i <= 2; i += 0.1) {
		env.float_stack.clear();
		env.float_stack.push_back(i);
		op(env);
		REQUIRE(env.float_stack == std::vector<double>{std::tan(i)});
	}
}
