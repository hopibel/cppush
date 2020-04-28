#include <catch2/catch.hpp>

#include "exec_ops.h"
#include "code.h"
#include "env.h"
#include "instruction.h"
#include "literal.h"

#include <memory>
#include <vector>

TEST_CASE("Instruction: EXEC.DO*RANGE") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_do_range, "EXEC.DO*RANGE");

	cppush::CodeList body;
	env.exec_stack.push_back(std::make_shared<cppush::CodeList>(body));

	SECTION("index == dest") {
		env.int_stack.insert(env.int_stack.end(), {0, 0});
		op(env);
		REQUIRE(env.int_stack == std::vector<int>{0});
		REQUIRE(env.exec_stack.size() == 1);
		REQUIRE(*env.exec_stack.back() == body);
	}

	SECTION("index < dest") {
		env.int_stack.insert(env.int_stack.end(), {0, 9});
		op(env);
		REQUIRE(env.int_stack == std::vector<int>{0});
		REQUIRE(env.exec_stack.size() == 2);
		auto& rcall = env.exec_stack[0]->get_stack();
		REQUIRE(rcall.size() == 4);
		REQUIRE(*rcall[0] == cppush::Literal<int>(1));
		REQUIRE(*rcall[1] == cppush::Literal<int>(9));
		REQUIRE(*rcall[2] == op);
		REQUIRE(*rcall[3] == body);
		REQUIRE(*env.exec_stack[1] == body);
	}

	SECTION("index > dest") {
		env.int_stack.insert(env.int_stack.end(), {9, 0});
		op(env);
		REQUIRE(env.int_stack == std::vector<int>{9});
		REQUIRE(env.exec_stack.size() == 2);
		auto& rcall = env.exec_stack[0]->get_stack();
		REQUIRE(rcall.size() == 4);
		REQUIRE(*rcall[0] == cppush::Literal<int>(8));
		REQUIRE(*rcall[1] == cppush::Literal<int>(0));
		REQUIRE(*rcall[2] == op);
		REQUIRE(*rcall[3] == body);
		REQUIRE(*env.exec_stack[1] == body);
	}
}

TEST_CASE("Instruction: EXEC.DO*COUNT") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_do_count, "EXEC.DO*COUNT");
	cppush::Instruction do_range(cppush::exec_do_range, "EXEC.DO*RANGE");
	cppush::CodeList body;
	env.exec_stack.push_back(std::make_shared<cppush::CodeList>(body));

	int count;

	SECTION("count < 1") {
		count = 0;
		env.int_stack.push_back(count);
		op(env);
		REQUIRE(env.int_stack == std::vector<int>{count});
		REQUIRE(env.exec_stack.size() == 1);
		REQUIRE(*env.exec_stack.back() == body);
	}

	SECTION("count > 0") {
		count = 2;
		env.int_stack.push_back(count);
		op(env);
		REQUIRE(env.int_stack.empty());
		REQUIRE(env.exec_stack.size() == 1);
		auto& rcall = env.exec_stack.back()->get_stack();
		REQUIRE(rcall.size() == 4);
		REQUIRE(*rcall[0] == cppush::Literal<int>(0));
		REQUIRE(*rcall[1] == cppush::Literal<int>(count - 1));
		REQUIRE(*rcall[2] == do_range);
		REQUIRE(*rcall[3] == body);
	}
}

TEST_CASE("Instruction: EXEC.DO*TIMES") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_do_times, "EXEC.DO*TIMES");
	cppush::Instruction do_range(cppush::exec_do_range, "EXEC.DO*RANGE");
	cppush::Instruction pop_insn(cppush::protected_pop<int>, "INTEGER.POP");
	cppush::CodeList body;
	env.exec_stack.push_back(std::make_shared<cppush::CodeList>(body));

	int times;

	SECTION("times < 1") {
		times = 0;
		env.int_stack.push_back(times);
		op(env);
		REQUIRE(env.int_stack == std::vector<int>{times});
		REQUIRE(env.exec_stack.size() == 1);
		REQUIRE(*env.exec_stack.back() == body);
	}

	SECTION("times > 0") {
		times = 2;
		env.int_stack.push_back(times);
		op(env);
		REQUIRE(env.int_stack.empty());
		REQUIRE(env.exec_stack.size() == 1);
		auto& rcall = env.exec_stack.back()->get_stack();
		REQUIRE(rcall.size() == 4);
		REQUIRE(*rcall[0] == cppush::Literal<int>(0));
		REQUIRE(*rcall[1] == cppush::Literal<int>(times - 1));
		REQUIRE(*rcall[2] == do_range);
		auto& pop_code = rcall[3]->get_stack();
		REQUIRE(pop_code.size() == 2);
		REQUIRE(*pop_code[0] == pop_insn);
		REQUIRE(*pop_code[1] == body);
	}
}

TEST_CASE("Instruction: EXEC.IF") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_if, "EXEC.IF");
	cppush::CodeList branchA;
	cppush::CodeList branchB({std::make_shared<cppush::CodeList>(branchA)});
	env.exec_stack.push_back(std::make_shared<cppush::CodeList>(branchB));
	env.exec_stack.push_back(std::make_shared<cppush::CodeList>(branchA));

	REQUIRE(!(branchA == branchB));

	SECTION("true") {
		env.bool_stack.push_back(true);
		op(env);
		REQUIRE(env.bool_stack.empty());
		REQUIRE(env.exec_stack.size() == 1);
		REQUIRE(*env.exec_stack.back() == branchA);
	}

	SECTION("false") {
		env.bool_stack.push_back(false);
		op(env);
		REQUIRE(env.bool_stack.empty());
		REQUIRE(env.exec_stack.size() == 1);
		REQUIRE(*env.exec_stack.back() == branchB);
	}
}

TEST_CASE("Instruction: EXEC.K") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_k, "EXEC.K");
	cppush::CodeList branchA;
	cppush::CodeList branchB({std::make_shared<cppush::CodeList>(branchA)});

	env.exec_stack.push_back(std::make_shared<cppush::CodeList>(branchB));
	env.exec_stack.push_back(std::make_shared<cppush::CodeList>(branchA));

	REQUIRE(!(branchA == branchB));

	// Before:	A B
	// After:	A
	op(env);
	REQUIRE(env.exec_stack.size() == 1);
	REQUIRE(*env.exec_stack.back() == branchA);
}

TEST_CASE("Instruction: EXEC.S") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_s, "EXEC.S");
	cppush::CodeList a;
	cppush::CodeList b({std::make_shared<cppush::CodeList>(a)});
	cppush::CodeList c({std::make_shared<cppush::CodeList>(b)});

	env.exec_stack.push_back(std::make_shared<cppush::CodeList>(c));
	env.exec_stack.push_back(std::make_shared<cppush::CodeList>(b));
	env.exec_stack.push_back(std::make_shared<cppush::CodeList>(a));

	REQUIRE(!(a == b));
	REQUIRE(!(a == c));
	REQUIRE(!(b == c));

	// Before:	A B C
	// After:	A C (B C)
	op(env);
	REQUIRE(env.exec_stack.size() == 3);
	REQUIRE(env.exec_stack[0]->get_stack().size() == 2);

	auto bc = cppush::CodeList(std::vector<std::shared_ptr<cppush::Code>>{
			std::make_shared<cppush::CodeList>(b),
			std::make_shared<cppush::CodeList>(c)
			});
	REQUIRE(*env.exec_stack[0] == bc);
	REQUIRE(*env.exec_stack[1] == c);
	REQUIRE(*env.exec_stack[2] == a);
}

TEST_CASE("Instruction: EXEC.Y") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_y, "EXEC.Y");
	cppush::CodeList body;

	env.exec_stack.push_back(std::make_shared<cppush::CodeList>(body));

	// Before:	A
	// After:	A (EXEC.Y A)
	op(env);
	REQUIRE(env.exec_stack.size() == 2);
	auto& rcall = env.exec_stack[0]->get_stack();
	REQUIRE(rcall.size() == 2);
	REQUIRE(*rcall[0] == op);
	REQUIRE(*rcall[1] == body);
	REQUIRE(*env.exec_stack[1] == body);
}
