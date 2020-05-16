#include <catch2/catch.hpp>

#include "exec_ops.h"
#include "code.h"
#include "env.h"
#include "instruction.h"
#include "literal.h"

#include <memory>
#include <vector>

TEST_CASE("Instruction: exec_do*range") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_do_range, "exec_do*range");

	cppush::CodeList body;
	env.push<cppush::Exec>(&body);

	SECTION("index == dest") {
		env.get_stack<int>().insert(env.get_stack<int>().end(), {0, 0});
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{0});
		REQUIRE(env.get_stack<cppush::Exec>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Exec>().back() == body);
	}

	SECTION("index < dest") {
		env.get_stack<int>().insert(env.get_stack<int>().end(), {0, 9});
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{0});
		REQUIRE(env.get_stack<cppush::Exec>().size() == 2);
		auto& rcall = env.get_stack<cppush::Exec>()[0]->get_stack();
		REQUIRE(rcall.size() == 4);
		REQUIRE(*rcall[0] == cppush::Literal<int>(1));
		REQUIRE(*rcall[1] == cppush::Literal<int>(9));
		REQUIRE(*rcall[2] == op);
		REQUIRE(*rcall[3] == body);
		REQUIRE(*env.get_stack<cppush::Exec>()[1] == body);
	}

	SECTION("index > dest") {
		env.get_stack<int>().insert(env.get_stack<int>().end(), {9, 0});
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{9});
		REQUIRE(env.get_stack<cppush::Exec>().size() == 2);
		auto& rcall = env.get_stack<cppush::Exec>()[0]->get_stack();
		REQUIRE(rcall.size() == 4);
		REQUIRE(*rcall[0] == cppush::Literal<int>(8));
		REQUIRE(*rcall[1] == cppush::Literal<int>(0));
		REQUIRE(*rcall[2] == op);
		REQUIRE(*rcall[3] == body);
		REQUIRE(*env.get_stack<cppush::Exec>()[1] == body);
	}
}

TEST_CASE("Instruction: exec_do*count") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_do_count, "exec_do*count");
	cppush::Instruction do_range(cppush::exec_do_range, "exec_do*range");
	cppush::CodeList body;
	env.push<cppush::Exec>(&body);

	int count;

	SECTION("count < 1") {
		count = 0;
		env.push<int>(count);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{count});
		REQUIRE(env.get_stack<cppush::Exec>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Exec>().back() == body);
	}

	SECTION("count > 0") {
		count = 2;
		env.push<int>(count);
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Exec>().size() == 1);
		auto& rcall = env.get_stack<cppush::Exec>().back()->get_stack();
		REQUIRE(rcall.size() == 4);
		REQUIRE(*rcall[0] == cppush::Literal<int>(0));
		REQUIRE(*rcall[1] == cppush::Literal<int>(count - 1));
		REQUIRE(*rcall[2] == do_range);
		REQUIRE(*rcall[3] == body);
	}
}

TEST_CASE("Instruction: exec_do*times") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_do_times, "exec_do*times");
	cppush::Instruction do_range(cppush::exec_do_range, "exec_do*range");
	cppush::Instruction pop_insn(cppush::protected_pop<int>, "integer_pop");
	cppush::CodeList body;
	env.push<cppush::Exec>(&body);

	int times;

	SECTION("times < 1") {
		times = 0;
		env.push<int>(times);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{times});
		REQUIRE(env.get_stack<cppush::Exec>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Exec>().back() == body);
	}

	SECTION("times > 0") {
		times = 2;
		env.push<int>(times);
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Exec>().size() == 1);
		auto& rcall = env.get_stack<cppush::Exec>().back()->get_stack();
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

TEST_CASE("Instruction: exec_if") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_if, "exec_if");
	cppush::CodeList branchA;
	cppush::CodeList branchB({ &branchA });
	env.push<cppush::Exec>(&branchB);
	env.push<cppush::Exec>(&branchA);

	REQUIRE(!(branchA == branchB));

	SECTION("true") {
		env.push<bool>(true);
		op(env);
		REQUIRE(env.get_stack<bool>().empty());
		REQUIRE(env.get_stack<cppush::Exec>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Exec>().back() == branchA);
	}

	SECTION("false") {
		env.push<bool>(false);
		op(env);
		REQUIRE(env.get_stack<bool>().empty());
		REQUIRE(env.get_stack<cppush::Exec>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Exec>().back() == branchB);
	}
}

TEST_CASE("Instruction: exec_k") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_k, "exec_k");
	cppush::CodeList branchA;
	cppush::CodeList branchB({ &branchA });

	env.push<cppush::Exec>(&branchB);
	env.push<cppush::Exec>(&branchA);

	REQUIRE(!(branchA == branchB));

	// Before:	A B
	// After:	A
	op(env);
	REQUIRE(env.get_stack<cppush::Exec>().size() == 1);
	REQUIRE(*env.get_stack<cppush::Exec>().back() == branchA);
}

TEST_CASE("Instruction: exec_s") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_s, "exec_s");
	cppush::CodeList a;
	cppush::CodeList b({ &a });
	cppush::CodeList c({ &b });

	env.push<cppush::Exec>(&c);
	env.push<cppush::Exec>(&b);
	env.push<cppush::Exec>(&a);

	REQUIRE(!(a == b));
	REQUIRE(!(a == c));
	REQUIRE(!(b == c));

	// Before:	A B C
	// After:	A C (B C)
	op(env);
	REQUIRE(env.get_stack<cppush::Exec>().size() == 3);
	REQUIRE(env.get_stack<cppush::Exec>()[0]->get_stack().size() == 2);

	auto bc = cppush::CodeList({ &b, &c });
	REQUIRE(*env.get_stack<cppush::Exec>()[0] == bc);
	REQUIRE(*env.get_stack<cppush::Exec>()[1] == c);
	REQUIRE(*env.get_stack<cppush::Exec>()[2] == a);
}

TEST_CASE("Instruction: exec_y") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_y, "exec_y");
	cppush::CodeList body;

	env.push<cppush::Exec>(&body);

	// Before:	A
	// After:	A (EXEC.Y A)
	op(env);
	REQUIRE(env.get_stack<cppush::Exec>().size() == 2);
	auto& rcall = env.get_stack<cppush::Exec>()[0]->get_stack();
	REQUIRE(rcall.size() == 2);
	REQUIRE(*rcall[0] == op);
	REQUIRE(*rcall[1] == body);
	REQUIRE(*env.get_stack<cppush::Exec>()[1] == body);
}
