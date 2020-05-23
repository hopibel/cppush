#include "code.h"

#include <catch2/catch.hpp>

using namespace cppush;

TEST_CASE("is_atom(Code) == !is_list(Code)") {
	Literal code(1);
	REQUIRE(is_atom(code));
	REQUIRE(is_list(code) == false);
	REQUIRE(is_atom(CodeList({ code })) == false);
	REQUIRE(is_list(CodeList({ code })));
}
