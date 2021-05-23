#include "cppush/code.hpp"
#include "cppush/number_ops.hpp"
#include "cppush/state.hpp"

#include <catch2/catch.hpp>
#include <memory>

TEST_CASE("Run number_add with (1 2 3) number stack") {
	cppush::State push;

	auto add = std::make_shared<cppush::Instruction>(cppush::number_add);
	std::vector<std::shared_ptr<cppush::Code>> program {add, add};
	push.push<double>(3);
	push.push<double>(2);
	push.push<double>(1);
	push.run(program);

	auto& number_stack = push.get_stack<double>();
	REQUIRE(number_stack.size() == 1);
	REQUIRE(number_stack.at(0) == 6);
}