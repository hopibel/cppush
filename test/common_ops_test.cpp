#include <catch2/catch.hpp>

#include "code.h"
#include "common_ops.h"
#include "env.h"
#include "instruction.h"

#include "memory"
#include "vector"

// TODO(hopibel): test helper function for generating test data
// generate_test_values<type>(int len)

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

TEMPLATE_TEST_CASE("Instruction: *.=", "", cppush::Code_ptr, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::equal<TestType>, "=");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>(std::vector<cppush::Code_ptr>{a});

	SECTION("Equal") {
		env.push<TestType>(a);
		env.push<TestType>(a);
		op(env);

		REQUIRE(env.get_stack<bool>() == std::vector<bool>{true});
		REQUIRE(env.get_stack<TestType>().empty());
	}

	SECTION("Not equal") {
		env.push<TestType>(a);
		env.push<TestType>(b);
		op(env);

		REQUIRE(env.get_stack<bool>() == std::vector<bool>{false});
		REQUIRE(env.get_stack<TestType>().empty());
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

TEMPLATE_TEST_CASE("Instruction: *.POP", "", cppush::Code_ptr, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::protected_pop<TestType>, "POP");

	auto a = std::make_shared<cppush::CodeList>();

	env.push<TestType>(a);
	op(env);
	REQUIRE(env.get_stack<TestType>().empty());
}

TEMPLATE_TEST_CASE("Instruction: *.DUP", "", bool, int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::dup<TestType>, "DUP");

	TestType a = static_cast<TestType>(4.20);

	env.push<TestType>(a);
	op(env);
	REQUIRE(env.get_stack<TestType>() == std::vector<TestType>{a, a});
}

TEMPLATE_TEST_CASE("Instruction: *.DUP", "", cppush::Code_ptr, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::dup<TestType>, "DUP");

	auto a = std::make_shared<cppush::CodeList>();

	env.push<TestType>(a);
	op(env);
	REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{a, a});
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

TEMPLATE_TEST_CASE("Instruction: *.FLUSH", "", cppush::Code_ptr, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::flush<TestType>, "FLUSH");

	auto a = std::make_shared<cppush::CodeList>();

	env.push<TestType>(a);
	env.push<TestType>(a);
	op(env);
	REQUIRE(env.get_stack<TestType>().empty());
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

TEMPLATE_TEST_CASE("Instruction: *.ROT", "", cppush::Code_ptr, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::rot<TestType>, "CODE.ROT");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();
	auto c = std::make_shared<cppush::CodeList>();

	env.push<TestType>(c);
	env.push<TestType>(b);
	env.push<TestType>(a);
	op(env);
	REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{
			b, a, c});
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

TEMPLATE_TEST_CASE("Instruction: *.SHOVE", "", cppush::Code_ptr, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::shove<TestType>, "SHOVE");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();
	auto c = std::make_shared<cppush::CodeList>();

	env.push<TestType>(c);
	env.push<TestType>(b);
	env.push<TestType>(a);

	auto int_stack_size = env.get_stack<int>().size();

	SECTION("Zero index") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{c, b, a});
	}

	SECTION("Negative index") {
		env.push<int>(-1);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{c, b, a});
	}

	SECTION("Positive index") {
		env.push<int>(2);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{a, c, b});
	}

	SECTION("Positive out of bounds index") {
		env.push<int>(4);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{a, c, b});
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

TEMPLATE_TEST_CASE("Instruction: *.STACKDEPTH", "", cppush::Code_ptr, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::stackdepth<TestType>, "STACKDEPTH");

	auto a = std::make_shared<cppush::CodeList>();

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

TEMPLATE_TEST_CASE("Instruction: *.SWAP", "", cppush::Code_ptr, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::swap<TestType>, "SWAP");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();

	env.push<TestType>(b);
	env.push<TestType>(a);
	op(env);
	REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{a, b});
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

TEMPLATE_TEST_CASE("Instruction: *.YANK", "", cppush::Code_ptr, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::yank<TestType>, "YANK");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();
	auto c = std::make_shared<cppush::CodeList>();

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
		REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{c, b, a});
	}

	SECTION("Negative index") {
		env.push<int>(-1);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{c, b, a});
	}

	SECTION("Positive index") {
		env.push<int>(2);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{b, a, c});
	}

	SECTION("Positive out of bounds index") {
		env.push<int>(4);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{b, a, c});
	}
}

TEMPLATE_TEST_CASE("Instruction: *.YANKDUP", "", bool, int, double) {
	cppush::Env env;
	cppush::Instruction op(cppush::yankdup<TestType>, "YANKDUP");

	TestType a = static_cast<TestType>(4.20);
	TestType b = static_cast<TestType>(0.0);
	TestType c = static_cast<TestType>(3.4);

	SECTION("Insufficient items on stack should be NOOP") {
		env.push<int>(0);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{0});
	}

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

TEMPLATE_TEST_CASE("Instruction: *.YANKDUP", "", cppush::Code_ptr, cppush::Exec) {
	cppush::Env env;
	cppush::Instruction op(cppush::yankdup<TestType>, "YANKDUP");

	auto a = std::make_shared<cppush::CodeList>();
	auto b = std::make_shared<cppush::CodeList>();
	auto c = std::make_shared<cppush::CodeList>();

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
		REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{c, b, a, a});
	}

	SECTION("Negative index") {
		env.push<int>(-1);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{c, b, a, a});
	}

	SECTION("Positive index") {
		env.push<int>(2);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{c, b, a, c});
	}

	SECTION("Positive out of bounds index") {
		env.push<int>(4);
		op(env);
		REQUIRE(env.get_stack<int>().size() == int_stack_size);
		REQUIRE(env.get_stack<TestType>() == std::vector<cppush::Code_ptr>{c, b, a, c});
	}
}
