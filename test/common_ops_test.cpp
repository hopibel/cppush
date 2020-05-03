#include <catch2/catch.hpp>

#include "test_utils.h"

#include "code.h"
#include "common_ops.h"
#include "env.h"
#include "instruction.h"

#include <algorithm>
#include <memory>
#include <vector>

TEMPLATE_TEST_CASE("Instruction: *.=", "", bool, int, double, cppush::Code, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::equal<TestType>, "=");

	auto vals = generate_test_values<TestType>(2);
	bool expected;

	SECTION("Equal") {
		env.push<TestType>(vals[0]);
		env.push<TestType>(vals[0]);
		expected = true;
	}

	SECTION("Not equal") {
		env.push<TestType>(vals[0]);
		env.push<TestType>(vals[1]);
		expected = false;
	}

	op(env);
	REQUIRE(env.pop<bool>() == expected);
	REQUIRE(env.get_stack<TestType>().empty());
}

TEMPLATE_TEST_CASE("Instruction: *.POP", "", bool, int, double, cppush::Code, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::protected_pop<TestType>, "POP");

	auto val = generate_test_values<TestType>(1)[0];

	env.push<TestType>(val);
	op(env);
	REQUIRE(env.get_stack<TestType>().empty());
}

TEMPLATE_TEST_CASE("Instruction: *.DUP", "", bool, int, double, cppush::Code, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::dup<TestType>, "DUP");

	auto vals = generate_test_values<TestType>(1);
	auto duped = vals;
	duped.push_back(duped[0]);

	env.push<TestType>(vals[0]);
	op(env);
	REQUIRE(env.get_stack<TestType>() == duped);
}

TEMPLATE_TEST_CASE("Instruction: *.FLUSH", "", bool, int, double, cppush::Code, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::flush<TestType>, "FLUSH");

	auto val = generate_test_values<TestType>(1)[0];

	env.push<TestType>(val);
	env.push<TestType>(val);
	op(env);
	REQUIRE(env.get_stack<TestType>().empty());
}

TEMPLATE_TEST_CASE("Instruction: *.ROT", "", bool, int, double, cppush::Code, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::rot<TestType>, "ROT");

	auto vals = generate_test_values<TestType>(3);

	env.push<TestType>(vals[0]);
	env.push<TestType>(vals[1]);
	env.push<TestType>(vals[2]);
	op(env);

	vals = {vals[1], vals[2], vals[0]};
	REQUIRE(env.get_stack<TestType>() == vals);
}

TEMPLATE_TEST_CASE("Instruction: *.SHOVE", "", bool, int, double, cppush::Code, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::shove<TestType>, "SHOVE");

	auto vals = generate_test_values<TestType>(3);

	env.push<TestType>(vals[0]);
	env.push<TestType>(vals[1]);
	env.push<TestType>(vals[2]);

	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero or negative index") {
		SECTION("Zero index") { env.push<int>(0); }
		SECTION("Negative index") { env.push<int>(-1); }
	}

	SECTION("Positive index, max or out of bounds") {
		SECTION("Positive index") { env.push<int>(2); }
		SECTION("Positive out of bounds index") { env.push<int>(4); }
		vals = {vals[2], vals[0], vals[1]};
	}

	op(env);
	REQUIRE(env.get_stack<int>().size() == int_stack_size);
	REQUIRE(env.get_stack<TestType>() == vals);
}

TEMPLATE_TEST_CASE("Instruction: *.STACKDEPTH", "", bool, int, double, cppush::Code, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::stackdepth<TestType>, "STACKDEPTH");

	auto vals = generate_test_values<TestType>(3);
	int expected;

	SECTION("Empty") { expected = 0; }
	SECTION("3 items") {
		env.get_stack<TestType>().insert(
			env.get_stack<TestType>().end(), vals.begin(), vals.end());
		expected = 3;
	}

	op(env);
	REQUIRE(env.pop<int>() == expected);
}

TEMPLATE_TEST_CASE("Instruction: *.SWAP", "", bool, int, double, cppush::Code, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::swap<TestType>, "SWAP");

	auto vals = generate_test_values<TestType>(2);

	env.push<TestType>(vals[0]);
	env.push<TestType>(vals[1]);
	op(env);
	vals = {vals[1], vals[0]};
	REQUIRE(env.get_stack<TestType>() == vals);
}

TEMPLATE_TEST_CASE("Instruction: *.YANK", "", bool, int, double, cppush::Code, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::yank<TestType>, "YANK");

	SECTION("Insufficient items on stack should be NOOP") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{0});
		return;
	}

	auto vals = generate_test_values<TestType>(3);

	for (auto el : vals) { env.push<TestType>(el); }
	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero or negative index") {
		SECTION("Zero index") { env.push<int>(0); }
		SECTION("Negative index") { env.push<int>(-1); }
	}

	SECTION("Max or out of bounds index") {
		SECTION("Positive index") { env.push<int>(2); }
		SECTION("Positive out of bounds index") { env.push<int>(4); }
		std::rotate(vals.begin(), vals.begin()+1, vals.end());
	}

	op(env);
	REQUIRE(env.get_stack<int>().size() == int_stack_size);
	REQUIRE(env.get_stack<TestType>() == vals);
}

TEMPLATE_TEST_CASE("Instruction: *.YANKDUP", "", bool, int, double, cppush::Code, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::yankdup<TestType>, "YANKDUP");

	SECTION("Insufficient items on stack should be NOOP") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{0});
		return;
	}

	auto vals = generate_test_values<TestType>(3);

	for (auto el : vals) { env.push<TestType>(el); }
	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero or negative index") {
		SECTION("Zero index") { env.push<int>(0); }
		SECTION("Negative index") { env.push<int>(-1); }
		vals.push_back(vals.back());
	}

	SECTION("Max or out of bounds index") {
		SECTION("Positive index") { env.push<int>(2); }
		SECTION("Positive out of bounds index") { env.push<int>(4); }
		vals.push_back(vals[0]);
	}

	op(env);
	REQUIRE(env.get_stack<int>().size() == int_stack_size);
	REQUIRE(env.get_stack<TestType>() == vals);
}
