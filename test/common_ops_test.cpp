#include <catch2/catch.hpp>

#include "test_utils.h"

#include "code.h"
#include "common_ops.h"
#include "env.h"

#include <algorithm>
#include <memory>
#include <vector>

using namespace cppush;

TEMPLATE_TEST_CASE("Instruction: equal", "", bool, int, double, Code, Exec) {
	Env env;
	Instruction op(equal<TestType>, "equal");
	auto vec = generate_test_values<TestType>(2);
	bool expected;

	SECTION("A A -> true") {
		env.push<TestType>(vec[0]);
		env.push<TestType>(vec[0]);
		expected = true;
	}
	SECTION("B A -> false") {
		env.push<TestType>(vec[0]);
		env.push<TestType>(vec[1]);
		expected = false;
	}
	op(env);
	REQUIRE(env.pop<bool>() == expected);
	REQUIRE(env.get_stack<TestType>().empty());
}

TEMPLATE_TEST_CASE("Instruction: pop", "", bool, int, double, Code, Exec) {
	Env env;
	Instruction op(protected_pop<TestType>, "pop");

	auto val = generate_test_values<TestType>(1);
	env.push<TestType>(val[0]);
	op(env);
	REQUIRE(env.get_stack<TestType>().empty());
}

TEMPLATE_TEST_CASE("Instruction: dup", "", bool, int, double, Code, Exec) {
	Env env;
	Instruction op(dup<TestType>, "dup");

	auto val = generate_test_values<TestType>(1);
	val.push_back(val[0]);
	env.push<TestType>(val[0]);
	op(env);
	REQUIRE(env.get_stack<TestType>() == val);
}

TEMPLATE_TEST_CASE("Instruction: flush", "", bool, int, double, Code, Exec) {
	Env env;
	Instruction op(flush<TestType>, "flush");

	auto vec = generate_test_values<TestType>(4);
	for (auto el : vec) {
		env.push<TestType>(el);
	}
	op(env);
	REQUIRE(env.get_stack<TestType>().empty());
}

TEMPLATE_TEST_CASE("Instruction: rot", "", bool, int, double, Code, Exec) {
	Env env;
	Instruction op(rot<TestType>, "rot");

	auto vec = generate_test_values<TestType>(4);
	auto expected = vec;
	std::rotate(expected.begin() + 1, expected.begin() + 2, expected.end());

	for (auto el : vec) {
		env.push<TestType>(el);
	}
	op(env);
	REQUIRE(env.get_stack<TestType>() == expected);
}

TEMPLATE_TEST_CASE("Instruction: shove", "", bool, int, double, Code, Exec) {
	Env env;
	Instruction op(shove<TestType>, "shove");

	auto vec = generate_test_values<TestType>(3);
	env.push<TestType>(vec[0]);
	env.push<TestType>(vec[1]);
	env.push<TestType>(vec[2]);

	auto expected = vec;
	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero or negative index") {
		SECTION("Zero index") { env.push<int>(0); }
		SECTION("Negative index") { env.push<int>(-1); }
	}
	SECTION("Positive index, max or out of bounds") {
		SECTION("Positive index") { env.push<int>(2); }
		SECTION("Positive out of bounds index") { env.push<int>(4); }
		expected = {vec[2], vec[0], vec[1]};
	}
	op(env);
	REQUIRE(env.get_stack<int>().size() == int_stack_size);
	REQUIRE(env.get_stack<TestType>() == expected);
}

TEMPLATE_TEST_CASE("Instruction: stackdepth", "", bool, int, double, Code, Exec) {
	Env env;
	Instruction op(stackdepth<TestType>, "stackdepth");

	auto vec = generate_test_values<TestType>(3);
	int expected;
	SECTION("Empty") { expected = 0; }
	SECTION("3 items") {
		env.push<TestType>(vec[0]);
		env.push<TestType>(vec[1]);
		env.push<TestType>(vec[2]);
		expected = 3;
	}
	auto int_stack_size = env.get_stack<int>().size();
	op(env);
	REQUIRE(env.pop<int>() == expected);
	REQUIRE(env.get_stack<int>().size() == int_stack_size);
}

TEMPLATE_TEST_CASE("Instruction: swap", "", bool, int, double, Code, Exec) {
	Env env;
	Instruction op(swap<TestType>, "swap");

	auto vec = generate_test_values<TestType>(2);
	auto expected = vec;
	expected = {vec[1], vec[0]};

	env.push<TestType>(vec[0]);
	env.push<TestType>(vec[1]);
	op(env);
	REQUIRE(env.get_stack<TestType>() == expected);
}

TEMPLATE_TEST_CASE("Instruction: yank", "", bool, int, double, Code, Exec) {
	Env env;
	Instruction op(yank<TestType>, "yank");

	SECTION("Insufficient items on stack should be NOOP") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{0});
		return;
	}

	auto vec = generate_test_values<TestType>(3);
	auto expected = vec;
	for (auto el : vec) { env.push<TestType>(el); }
	auto int_stack_size = env.get_stack<int>().size(); // workaround for int

	SECTION("Zero or negative index: A B C -> A B C") {
		SECTION("Zero index") { env.push<int>(0); }
		SECTION("Clamp negative to zero") { env.push<int>(-1); }
	}
	SECTION("Positive index") {
		SECTION("Positive index") { env.push<int>(2); }
		SECTION("Clamp to max index") { env.push<int>(4); }
		std::rotate(expected.begin(), expected.begin()+1, expected.end());
	}
	op(env);
	REQUIRE(env.get_stack<int>().size() == int_stack_size);
	REQUIRE(env.get_stack<TestType>() == expected);
}

TEMPLATE_TEST_CASE("Instruction: yankdup", "", bool, int, double, Code, Exec) {
	Env env;
	Instruction op(yankdup<TestType>, "yankdup");

	SECTION("Insufficient items on stack should be NOOP") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{0});
		return;
	}

	auto vec = generate_test_values<TestType>(3);
	auto expected = vec;
	for (auto el : vec) { env.push<TestType>(el); }
	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero or negative index") {
		SECTION("Zero index") { env.push<int>(0); }
		SECTION("Clamp negative to zero") { env.push<int>(-1); }
		expected.push_back(expected.back());
	}
	SECTION("Positive index") {
		SECTION("Positive index") { env.push<int>(2); }
		SECTION("Clamp to max index") { env.push<int>(4); }
		expected.push_back(expected[0]);
	}
	op(env);
	REQUIRE(env.get_stack<int>().size() == int_stack_size);
	REQUIRE(env.get_stack<TestType>() == expected);
}
