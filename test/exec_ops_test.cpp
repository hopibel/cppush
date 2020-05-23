#include <catch2/catch.hpp>

#include "test_utils.h"

#include "exec_ops.h"
#include "code.h"
#include "env.h"

#include <memory>
#include <vector>

using namespace cppush;

TEST_CASE("Instruction: exec_do*range") {
	Env env;
	Instruction op(exec_do_range, "exec_do*range");
	std::vector<Code> expected_exec;
	int expected_int;

	CodeList body;
	env.push<Exec>(body);

	SECTION("index == dest\n"
			"exec: (body) -> (body)\n"
			"int:  0 0 -> 0")
	{
		env.push<int>(0);
		env.push<int>(0);
		expected_int = 0;
		expected_exec = { body };
	}
	SECTION("index < dest\n"
			"exec: (body) -> (body) (1 9 exec_do*range (body))\n"
			"int:  9 0 -> 0")
	{
		env.push<int>(0);
		env.push<int>(9);
		expected_int = 0;
		expected_exec = { CodeList({ Literal(1), Literal(9), op, body }), body };
	}
	SECTION("index > dest\n"
			"exec: (body) -> (body) (8 0 exec_do*range (body))\n"
			"int:  0 9 -> 8")
	{
		env.push<int>(9);
		env.push<int>(0);
		expected_int = 9;
		expected_exec = { CodeList({ Literal(8), Literal(0), op, body }), body };
	}
	op(env);
	REQUIRE(env.get_stack<int>() == std::vector<int>{ expected_int });
	REQUIRE(env.get_stack<Exec>() == expected_exec);
}

TEST_CASE("Instruction: exec_do*count") {
	Env env;
	Instruction op(exec_do_count, "exec_do*count");
	Instruction do_range(exec_do_range, "exec_do*range");

	CodeList body;
	env.push<Exec>(body);

	int count;
	SECTION("count < 1 is noop") {
		count = 0;
		env.push<int>(count);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{count});
		REQUIRE(env.get_stack<Exec>() == std::vector<Code>{body});
	}
	SECTION("count > 0\n"
			"exec: (body) -> (0 1 exec_do*range (body))\n"
			"int:  2 -> {}")
	{
		count = 2;
		env.push<int>(count);
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<Exec>() == std::vector<Code>{CodeList({
			Literal(0), Literal(count - 1), do_range, body
		})});
	}
}

TEST_CASE("Instruction: exec_do*times") {
	Env env;
	Instruction op(exec_do_times, "exec_do*times");
	Instruction do_range(exec_do_range, "exec_do*range");
	Instruction pop_insn(protected_pop<int>, "integer_pop");

	CodeList body;
	env.push<Exec>(body);
	int times;

	SECTION("times < 1 is noop") {
		times = 0;
		env.push<int>(times);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{times});
		REQUIRE(env.get_stack<Exec>() == std::vector<Code>{body});
	}
	SECTION("times > 0\n"
			"exec: (body) -> (0 1 exec_do*range (integer_pop (body)))\n"
			"int:  2 -> {}")
	{
		times = 2;
		env.push<int>(times);
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<Exec>() == std::vector<Code>{ CodeList({
			Literal(0), Literal(times - 1), do_range, CodeList({ pop_insn, body })
		}) });
	}
}

TEST_CASE("Instruction: exec_if") {
	Env env;
	Instruction op(exec_if, "exec_if");
	CodeList branchA;
	CodeList branchB({ Code(branchA) });
	CodeList expected;

	env.push<Exec>(branchB);
	env.push<Exec>(branchA);

	REQUIRE(!(branchA == branchB));

	SECTION("true\n"
			"exec: A B -> A\n"
			"bool: true -> {}")
	{
		env.push<bool>(true);
		expected = branchA;
	}
	SECTION("false\n"
			"exec: A B -> B\n"
			"bool: false -> {}")
	{
		env.push<bool>(false);
		expected = branchB;
	}
	op(env);
	REQUIRE(env.get_stack<bool>().empty());
	REQUIRE(env.get_stack<Exec>() == std::vector<Code>{expected});
}

TEST_CASE("Instruction: exec_k\nexec: A B -> A") {
	Env env;
	Instruction op(exec_k, "exec_k");
	CodeList branchA;
	CodeList branchB({ Code(branchA) });

	env.push<Exec>(branchB);
	env.push<Exec>(branchA);

	REQUIRE(!(branchA == branchB));
	op(env);
	REQUIRE(env.get_stack<Exec>() == std::vector<Code>{branchA});
}

TEST_CASE("Instruction: exec_s\nexec: A B C -> A C (B C)") {
	Env env;
	Instruction op(exec_s, "exec_s");
	auto vec = generate_test_values<Code>(3);

	env.push<Exec>(vec[2]);
	env.push<Exec>(vec[1]);
	env.push<Exec>(vec[0]);

	op(env);
	REQUIRE(env.get_stack<Exec>() == std::vector<Code>{
		CodeList({ vec[1], vec[2] }), vec[2], vec[0]
	});
}

TEST_CASE("Instruction: exec_y\nexec: A -> A (exec_y A)") {
	Env env;
	Instruction op(exec_y, "exec_y");

	CodeList body;
	env.push<Exec>(body);

	op(env);
	REQUIRE(env.get_stack<Exec>() == std::vector<Code>{
		CodeList({ op, body }), body
	});
}
