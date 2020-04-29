#include <catch2/catch.hpp>

#include "code.h"
#include "common_ops.h"
#include "env.h"
#include "instruction.h"

#include "memory"
#include "vector"

// bool, int, double
// exec, code

TEMPLATE_TEST_CASE("Instruction: *.=", "" , bool, int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::equal<TestType>, "=");

	TestType a = static_cast<TestType>(4.20);
	TestType b = static_cast<TestType>(0.0);

	SECTION("Equal") {
		env.push<TestType>(a);
		env.push<TestType>(a);
		op(env);

		REQUIRE(env.pop<bool>() == true);
		REQUIRE(env.get_stack<TestType>().empty());
	}

	SECTION("Not equal") {
		env.push<TestType>(a);
		env.push<TestType>(b);
		op(env);

		REQUIRE(env.pop<bool>() == false);
		REQUIRE(env.get_stack<TestType>().empty());
	}
}

TEST_CASE("Instruction: CODE.=") {
	cppush::Env env;
	cppush::Instruction op(cppush::equal<cppush::Code_ptr>, "CODE.=");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>(std::vector<cppush::Code_ptr>{a});

	SECTION("Equal") {
		env.push<cppush::Code_ptr>(a);
		env.push<cppush::Code_ptr>(a);
		op(env);

		REQUIRE(env.get_stack<bool>() == std::vector<bool>{true});
		REQUIRE(env.get_stack<cppush::Code_ptr>().empty());
	}

	SECTION("Not equal") {
		env.push<cppush::Code_ptr>(a);
		env.push<cppush::Code_ptr>(b);
		op(env);

		REQUIRE(env.get_stack<bool>() == std::vector<bool>{false});
		REQUIRE(env.get_stack<cppush::Code_ptr>().empty());
	}
}

TEST_CASE("Instruction: EXEC.=") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_equal, "EXEC.=");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>(std::vector<cppush::Code_ptr>{a});

	SECTION("Equal") {
		env.push_exec(a);
		env.push_exec(a);
		op(env);

		REQUIRE(env.get_exec_stack().empty());
		REQUIRE(env.get_stack<bool>() == std::vector<bool>{true});
	}

	SECTION("Not equal") {
		env.push_exec(a);
		env.push_exec(b);
		op(env);

		REQUIRE(env.get_exec_stack().empty());
		REQUIRE(env.get_stack<bool>() == std::vector<bool>{false});
	}
}

TEMPLATE_TEST_CASE("Instruction: *.POP", "", bool, int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::protected_pop<TestType>, "POP");

	TestType a = static_cast<TestType>(4.20);

	env.push<TestType>(a);
	op(env);
	REQUIRE(env.get_stack<TestType>().empty());
}

TEST_CASE("Instruction: CODE.POP") {
	cppush::Env env;
	cppush::Instruction op(cppush::protected_pop<cppush::Code_ptr>, "CODE.POP");

	auto a = std::make_shared<cppush::CodeList>();

	env.push<cppush::Code_ptr>(a);
	op(env);
	REQUIRE(env.get_stack<cppush::Code_ptr>().empty());
}

TEST_CASE("Instruction: EXEC.POP") {
	cppush::Env env;
	cppush::Instruction op(cppush::protected_exec_pop, "EXEC.POP");

	auto a = std::make_shared<cppush::CodeList>();

	env.push_exec(a);
	op(env);
	REQUIRE(env.get_exec_stack().empty());
}

TEMPLATE_TEST_CASE("Instruction: *.DUP", "", bool, int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::dup<TestType>, "DUP");

	TestType a = static_cast<TestType>(4.20);

	env.push<TestType>(a);
	op(env);
	REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{a, a});
}

TEST_CASE("Instruction: CODE.DUP") {
	cppush::Env env;
	cppush::Instruction op(cppush::dup<cppush::Code_ptr>, "CODE.DUP");

	auto a = std::make_shared<cppush::CodeList>();

	env.push<cppush::Code_ptr>(a);
	op(env);
	REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{a, a});
}

TEST_CASE("Instruction: EXEC.DUP") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_dup, "EXEC.DUP");

	auto a = std::make_shared<cppush::CodeList>();

	env.push_exec(a);
	op(env);
	REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{a, a});
}

TEMPLATE_TEST_CASE("Instruction: *.FLUSH", "", bool, int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::flush<TestType>, "FLUSH");

	TestType a = static_cast<TestType>(4.20);

	env.push<TestType>(a);
	env.push<TestType>(a);
	op(env);
	REQUIRE(env.get_stack<TestType>().empty());
}

TEST_CASE("Instruction: CODE.FLUSH") {
	cppush::Env env;
	cppush::Instruction op(cppush::flush<cppush::Code_ptr>, "CODE.FLUSH");

	auto a = std::make_shared<cppush::CodeList>();

	env.push<cppush::Code_ptr>(a);
	env.push<cppush::Code_ptr>(a);
	op(env);
	REQUIRE(env.get_stack<cppush::Code_ptr>().empty());
}

TEST_CASE("Instruction: EXEC.FLUSH") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_flush, "EXEC.FLUSH");

	auto a = std::make_shared<cppush::CodeList>();

	env.push_exec(a);
	env.push_exec(a);
	op(env);
	REQUIRE(env.get_exec_stack().empty());
}

TEMPLATE_TEST_CASE("Instruction: *.ROT", "", bool, int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::rot<TestType>, "ROT");

	TestType a = static_cast<TestType>(4.20);
	TestType b = static_cast<TestType>(0.0);
	TestType c = static_cast<TestType>(2.4);

	env.push<TestType>(c);
	env.push<TestType>(b);
	env.push<TestType>(a);
	op(env);
	REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{b, a, c});
}

TEST_CASE("Instruction: CODE.ROT") {
	cppush::Env env;
	cppush::Instruction op(cppush::rot<cppush::Code_ptr>, "CODE.ROT");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();
	auto c = std::make_shared<cppush::CodeList>();

	env.push<cppush::Code_ptr>(c);
	env.push<cppush::Code_ptr>(b);
	env.push<cppush::Code_ptr>(a);
	op(env);
	REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{
			b, a, c});
}

TEST_CASE("Instruction: EXEC.ROT") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_rot, "EXEC.ROT");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();
	auto c = std::make_shared<cppush::CodeList>();

	env.push_exec(c);
	env.push_exec(b);
	env.push_exec(a);
	op(env);
	REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{b, a, c});
}

TEMPLATE_TEST_CASE("Instruction: *.SHOVE", "", bool, int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::shove<TestType>, "SHOVE");

	TestType a = static_cast<TestType>(4.20);
	TestType b = static_cast<TestType>(0.0);
	TestType c = static_cast<TestType>(2.4);

	env.push<TestType>(c);
	env.push<TestType>(b);
	env.push<TestType>(a);

	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero index") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{c, b, a});
	}

	SECTION("Negative index") {
		env.push<int>(-1);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{c, b, a});
	}

	SECTION("Positive index") {
		env.push<int>(2);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{a, c, b});
	}

	SECTION("Positive out of bounds index") {
		env.push<int>(4);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{a, c, b});
	}
}

TEST_CASE("Instruction: CODE.SHOVE") {
	cppush::Env env;
	cppush::Instruction op(cppush::shove<cppush::Code_ptr>, "CODE.SHOVE");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();
	auto c = std::make_shared<cppush::CodeList>();

	env.push<cppush::Code_ptr>(c);
	env.push<cppush::Code_ptr>(b);
	env.push<cppush::Code_ptr>(a);

	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero index") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{c, b, a});
	}

	SECTION("Negative index") {
		env.push<int>(-1);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{c, b, a});
	}

	SECTION("Positive index") {
		env.push<int>(2);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{a, c, b});
	}

	SECTION("Positive out of bounds index") {
		env.push<int>(4);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{a, c, b});
	}
}

// TODO(hopibel): should be able to refactor
TEST_CASE("Instruction: EXEC.SHOVE") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_shove, "EXEC.SHOVE");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();
	auto c = std::make_shared<cppush::CodeList>();

	env.push_exec(c);
	env.push_exec(b);
	env.push_exec(a);

	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero index") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{c, b, a});
	}

	SECTION("Negative index") {
		env.push<int>(-1);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{c, b, a});
	}

	SECTION("Positive index") {
		env.push<int>(2);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{a, c, b});
	}

	SECTION("Positive out of bounds index") {
		env.push<int>(4);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{a, c, b});
	}
}

TEMPLATE_TEST_CASE("Instruction: *.STACKDEPTH", "", bool, int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::stackdepth<TestType>, "STACKDEPTH");

	TestType a = static_cast<TestType>(4.20);

	SECTION("Empty") {
		op(env);
		REQUIRE(env.get_stack<int>().back() == 0);
	}

	SECTION("3 items") {
		env.push<TestType>(a);
		env.push<TestType>(a);
		env.push<TestType>(a);
		op(env);
		REQUIRE(env.get_stack<int>().back() == 3);
	}
}

TEST_CASE("Instruction: CODE.STACKDEPTH") {
	cppush::Env env;
	cppush::Instruction op(cppush::stackdepth<cppush::Code_ptr>, "CODE.STACKDEPTH");

	auto a = std::make_shared<cppush::CodeList>();

	SECTION("Empty") {
		op(env);
		REQUIRE(env.get_stack<int>().back() == 0);
	}

	SECTION("3 items") {
		env.push<cppush::Code_ptr>(a);
		env.push<cppush::Code_ptr>(a);
		env.push<cppush::Code_ptr>(a);
		op(env);
		REQUIRE(env.get_stack<int>().back() == 3);
	}
}

TEST_CASE("Instruction: EXEC.STACKDEPTH") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_stackdepth, "EXEC.STACKDEPTH");

	auto a = std::make_shared<cppush::CodeList>();

	SECTION("Empty") {
		op(env);
		REQUIRE(env.get_stack<int>().back() == 0);
	}

	SECTION("3 items") {
		env.push_exec(a);
		env.push_exec(a);
		env.push_exec(a);
		op(env);
		REQUIRE(env.get_stack<int>().back() == 3);
	}
}

TEMPLATE_TEST_CASE("Instruction: *.SWAP", "", bool, int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::swap<TestType>, "SWAP");

	TestType a = static_cast<TestType>(4.20);
	TestType b = static_cast<TestType>(0.0);

	env.push<TestType>(b);
	env.push<TestType>(a);
	op(env);
	REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{a, b});
}

TEST_CASE("Instruction: CODE.SWAP") {
	cppush::Env env;
	cppush::Instruction op(cppush::swap<cppush::Code_ptr>, "CODE.SWAP");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();

	env.push<cppush::Code_ptr>(b);
	env.push<cppush::Code_ptr>(a);
	op(env);
	REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{a, b});
}

TEST_CASE("Instruction: EXEC.SWAP") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_swap, "EXEC.SWAP");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();

	env.push_exec(b);
	env.push_exec(a);
	op(env);
	REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{a, b});
}

TEMPLATE_TEST_CASE("Instruction: *.YANK", "", bool, int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::yank<TestType>, "YANK");

	TestType a = static_cast<TestType>(4.20);
	TestType b = static_cast<TestType>(0.0);
	TestType c = static_cast<TestType>(2.4);

	SECTION("Insufficient items on stack should be NOOP") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{0});
	}

	env.push<TestType>(c);
	env.push<TestType>(b);
	env.push<TestType>(a);

	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero index") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{c, b, a});
	}

	SECTION("Negative index") {
		env.push<int>(-1);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{c, b, a});
	}

	SECTION("Positive index") {
		env.push<int>(2);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{b, a, c});
	}

	SECTION("Positive out of bounds index") {
		env.push<int>(4);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{b, a, c});
	}
}

TEST_CASE("Instruction: CODE.YANK") {
	cppush::Env env;
	cppush::Instruction op(cppush::yank<cppush::Code_ptr>, "CODE.YANK");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();
	auto c = std::make_shared<cppush::CodeList>();

	env.push<cppush::Code_ptr>(c);
	env.push<cppush::Code_ptr>(b);
	env.push<cppush::Code_ptr>(a);

	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero index") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{c, b, a});
	}

	SECTION("Negative index") {
		env.push<int>(-1);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{c, b, a});
	}

	SECTION("Positive index") {
		env.push<int>(2);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{b, a, c});
	}

	SECTION("Positive out of bounds index") {
		env.push<int>(4);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{b, a, c});
	}
}

// TODO(hopibel): should be able to refactor
TEST_CASE("Instruction: EXEC.YANK") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_yank, "EXEC.YANK");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();
	auto c = std::make_shared<cppush::CodeList>();

	env.push_exec(c);
	env.push_exec(b);
	env.push_exec(a);

	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero index") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{c, b, a});
	}

	SECTION("Negative index") {
		env.push<int>(-1);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{c, b, a});
	}

	SECTION("Positive index") {
		env.push<int>(2);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{b, a, c});
	}

	SECTION("Positive out of bounds index") {
		env.push<int>(4);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{b, a, c});
	}
}

TEMPLATE_TEST_CASE("Instruction: *.YANKDUP", "", bool, int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::yankdup<TestType>, "YANKDUP");

	TestType a = static_cast<TestType>(4.20);
	TestType b = static_cast<TestType>(0.0);
	TestType c = static_cast<TestType>(3.4);

	env.push<TestType>(c);
	env.push<TestType>(b);
	env.push<TestType>(a);

	SECTION("Zero index") {
		env.push<int>(0);
		op(env);
		REQUIRE((env.get_stack<int>().empty() || env.get_stack<int>().back() != 0));
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{c, b, a, a});
	}

	SECTION("Negative index") {
		env.push<int>(-1);
		op(env);
		REQUIRE((env.get_stack<int>().empty() || env.get_stack<int>().back() != -1));
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{c, b, a, a});
	}

	SECTION("Positive index") {
		env.push<int>(2);
		op(env);
		REQUIRE((env.get_stack<int>().empty() || env.get_stack<int>().back() != 2));
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{c, b, a, c});
	}

	SECTION("Positive out of bounds index") {
		env.push<int>(4);
		op(env);
		REQUIRE((env.get_stack<int>().empty() || env.get_stack<int>().back() != 4));
		REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{c, b, a, c});
	}
}

TEST_CASE("Instruction: CODE.YANKDUP") {
	cppush::Env env;
	cppush::Instruction op(cppush::yankdup<cppush::Code_ptr>, "CODE.YANKDUP");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();
	auto c = std::make_shared<cppush::CodeList>();

	env.push<cppush::Code_ptr>(c);
	env.push<cppush::Code_ptr>(b);
	env.push<cppush::Code_ptr>(a);

	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero index") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{c, b, a, a});
	}

	SECTION("Negative index") {
		env.push<int>(-1);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{c, b, a, a});
	}

	SECTION("Positive index") {
		env.push<int>(2);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{c, b, a, c});
	}

	SECTION("Positive out of bounds index") {
		env.push<int>(4);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<cppush::Code_ptr>() == std::vector<cppush::Code_ptr>{c, b, a, c});
	}
}

// TODO(hopibel): should be able to refactor
TEST_CASE("Instruction: EXEC.YANKDUP") {
	cppush::Env env;
	cppush::Instruction op(cppush::exec_yankdup, "EXEC.YANKDUP");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();
	auto c = std::make_shared<cppush::CodeList>();

	env.push_exec(c);
	env.push_exec(b);
	env.push_exec(a);

	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero index") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{c, b, a, a});
	}

	SECTION("Negative index") {
		env.push<int>(-1);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{c, b, a, a});
	}

	SECTION("Positive index") {
		env.push<int>(2);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{c, b, a, c});
	}

	SECTION("Positive out of bounds index") {
		env.push<int>(4);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_exec_stack() == std::vector<cppush::Code_ptr>{c, b, a, c});
	}
}
