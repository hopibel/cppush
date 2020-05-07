#include <catch2/catch.hpp>

#include "code.h"
#include "code_ops.h"
#include "env.h"
#include "instruction.h"
#include "literal.h"

#include <cmath>
#include <memory>
#include <vector>

#include <iostream>

TEST_CASE("Instruction: code_append") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_append, "code_append");

	SECTION("Both atoms"){
		auto first = std::make_shared<cppush::Literal<int>>(1);
		auto second = std::make_shared<cppush::Literal<int>>(1);
		env.get_stack<cppush::Code>().insert(env.get_stack<cppush::Code>().end(), {second, first});
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(env.get_stack<cppush::Code>().back()->is_list());
		auto& stack = env.get_stack<cppush::Code>().back()->get_stack();
		REQUIRE(stack[0] == second);
		REQUIRE(stack[1] == first);
	}

	SECTION("Both lists"){
		auto first = std::make_shared<cppush::CodeList>(
				std::vector<cppush::Code>{
					std::make_shared<cppush::Literal<int>>(1)
				});
		auto second = std::make_shared<cppush::CodeList>(
				std::vector<cppush::Code>{
					std::make_shared<cppush::Literal<int>>(2)
				});

		env.get_stack<cppush::Code>().insert(env.get_stack<cppush::Code>().end(), {second, first});
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(env.get_stack<cppush::Code>().back()->is_list());
		auto& stack = env.get_stack<cppush::Code>().back()->get_stack();
		REQUIRE(stack[0] == second->get_stack()[0]);
		REQUIRE(stack[1] == first->get_stack()[0]);
	}

	SECTION("Atom and list"){
		auto code_list = std::make_shared<cppush::CodeList>(
				std::vector<cppush::Code>{
					std::make_shared<cppush::Literal<int>>(1)
				});
		auto atom = std::make_shared<cppush::Literal<int>>(2);

		SECTION("Append list to atom") {
			env.get_stack<cppush::Code>().insert(env.get_stack<cppush::Code>().end(), {atom, code_list});
			op(env);
			REQUIRE(env.get_stack<cppush::Code>().size() == 1);
			REQUIRE(env.get_stack<cppush::Code>().back()->is_list());
			auto& stack = env.get_stack<cppush::Code>().back()->get_stack();
			REQUIRE(stack[0] == atom);
			REQUIRE(stack[1] == code_list->get_stack()[0]);
		}

		SECTION("Append atom to list") {
			env.get_stack<cppush::Code>().insert(env.get_stack<cppush::Code>().end(), {code_list, atom});
			op(env);
			REQUIRE(env.get_stack<cppush::Code>().size() == 1);
			REQUIRE(env.get_stack<cppush::Code>().back()->is_list());
			auto& stack = env.get_stack<cppush::Code>().back()->get_stack();
			REQUIRE(stack[0] == code_list->get_stack()[0]);
			REQUIRE(stack[1] == atom);
		}
	}
}

TEST_CASE("Instruction: code_atom") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_atom, "code_atom");

	SECTION("Atom") {
		auto atom = std::make_shared<cppush::Literal<int>>(0);
		env.push<cppush::Code>(atom);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.pop<bool>() == true);
		REQUIRE(env.get_stack<bool>().empty());
	}

	SECTION("List") {
		auto code_list = std::make_shared<cppush::CodeList>();
		env.push<cppush::Code>(code_list);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.pop<bool>() == false);
		REQUIRE(env.get_stack<bool>().empty());
	}
}

TEST_CASE("Instruction: code_car") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_car, "code_car");

	SECTION("Atom") {
		auto atom = std::make_shared<cppush::Literal<int>>(0);
		env.push<cppush::Code>(atom);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>() == std::vector<cppush::Code>{atom});
	}

	SECTION("List") {
		auto atom_a = std::make_shared<cppush::Literal<int>>(0);
		auto atom_b = std::make_shared<cppush::Literal<int>>(1);
		auto list_ab = std::vector<cppush::Code>{atom_a, atom_b};
		auto code_list = std::make_shared<cppush::CodeList>(list_ab);
		env.push<cppush::Code>(code_list);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>() == std::vector<cppush::Code>{atom_a});
	}
}

TEST_CASE("Instruction: code_cdr") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_cdr, "code_cdr");

	SECTION("Atom") {
		auto atom = std::make_shared<cppush::Literal<int>>(0);
		env.push<cppush::Code>(atom);
		op(env);
		// CDR on atom results in empty list
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == cppush::CodeList());
	}

	SECTION("List") {
		auto atom_a = std::make_shared<cppush::Literal<int>>(0);
		auto atom_b = std::make_shared<cppush::Literal<int>>(1);
		auto list_ab = std::vector<cppush::Code>{atom_a, atom_b};
		auto list_b = std::vector<cppush::Code>{atom_b};
		auto code_list = std::make_shared<cppush::CodeList>(list_ab);
		env.push<cppush::Code>(code_list);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == cppush::CodeList(list_b));
	}
}

TEST_CASE("Instruction: code_cons") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_cons, "code_cons");

	auto second = std::make_shared<cppush::Literal<int>>(1);

	SECTION("First item is an atom") {
		auto first = std::make_shared<cppush::Literal<int>>(0);
		env.push<cppush::Code>(second);
		env.push<cppush::Code>(first);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		auto& code_list = env.get_stack<cppush::Code>().back()->get_stack();
		REQUIRE(code_list.size() == 2);
		REQUIRE(*code_list[0] == *second);
		REQUIRE(*code_list[1] == *first);
	}

	SECTION("First item is a list") {
		auto first = std::make_shared<cppush::CodeList>(
				std::vector<cppush::Code>{std::make_shared<cppush::Literal<int>>(0)}
				);
		env.push<cppush::Code>(second);
		env.push<cppush::Code>(first);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		auto& code_list = env.get_stack<cppush::Code>().back()->get_stack();
		REQUIRE(code_list.size() == 2);
		REQUIRE(*code_list[0] == *second);
		REQUIRE(*code_list[1] == *first->get_stack()[0]);
	}
}

TEST_CASE("Instruction: code_container") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_container, "code_container");

	/*
	For example, if the top piece of code is "( B ( C ( A ) ) ( D ( A ) ) )" and the second piece of code is
	"( A )" then this pushes ( C ( A ) ). Pushes an empty list if there is no such container. 
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);
	auto d = std::make_shared<cppush::Literal<int>>(3);

	std::vector<cppush::Code> list_a{a};
	std::vector<cppush::Code> list_ca{c, std::make_shared<cppush::CodeList>(list_a)};
	std::vector<cppush::Code> list_bca{b, std::make_shared<cppush::CodeList>(list_ca)};
	std::vector<cppush::Code> list_da{d, std::make_shared<cppush::CodeList>(list_a)};

	std::vector<cppush::Code> list_full{
			std::make_shared<cppush::CodeList>(list_bca),
			std::make_shared<cppush::CodeList>(list_da)
	};

	auto container = std::make_shared<cppush::CodeList>(list_full);

	SECTION("Find container") {
		env.push<cppush::Code>(std::make_shared<cppush::CodeList>(list_a));
		env.push<cppush::Code>(container);
		op(env);

		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == cppush::CodeList(list_ca));
	}

	SECTION("Push empty list if no container") {
		env.push<cppush::Code>(container);
		env.push<cppush::Code>(std::make_shared<cppush::CodeList>(list_a));
		op(env);

		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == cppush::CodeList());
	}
}

TEST_CASE("Instruction: code_contains") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_contains, "code_contains");

	/*
	 * First:	( A )
	 * Second:	( B ( C ( A ) ) ( D ( A ) ) )
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);
	auto d = std::make_shared<cppush::Literal<int>>(3);

	std::vector<cppush::Code> list_a{a};
	std::vector<cppush::Code> list_ca{c, std::make_shared<cppush::CodeList>(list_a)};
	std::vector<cppush::Code> list_bca{b, std::make_shared<cppush::CodeList>(list_ca)};
	std::vector<cppush::Code> list_da{d, std::make_shared<cppush::CodeList>(list_a)};

	std::vector<cppush::Code> list_full{
			std::make_shared<cppush::CodeList>(list_bca),
			std::make_shared<cppush::CodeList>(list_da)
	};

	auto container = std::make_shared<cppush::CodeList>(list_full);

	SECTION("Subtree found") {
		env.push<cppush::Code>(container);
		env.push<cppush::Code>(std::make_shared<cppush::CodeList>(list_a));
		op(env);

		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.pop<bool>() == true);
		REQUIRE(env.get_stack<bool>().empty());
	}

	SECTION("Not a subtree") {
		// swap, so we have a superset instead of a subset
		env.push<cppush::Code>(std::make_shared<cppush::CodeList>(list_a));
		env.push<cppush::Code>(container);
		op(env);

		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.pop<bool>() == false);
		REQUIRE(env.get_stack<bool>().empty());
	}

	SECTION("Items are equal") {
		env.push<cppush::Code>(container);
		env.push<cppush::Code>(container);
		op(env);

		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.pop<bool>() == true);
		REQUIRE(env.get_stack<bool>().empty());
	}
}

TEST_CASE("Instruction: code_do") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_do, "code_do");

	std::shared_ptr<cppush::Instruction> pop_insn = std::make_shared<cppush::Instruction>(
			cppush::protected_pop<cppush::Code>, "code_pop");
	std::shared_ptr<cppush::CodeList> code_list = std::make_shared<cppush::CodeList>();

	env.push<cppush::Code>(code_list);
	op(env);
	REQUIRE(env.get_stack<cppush::Code>() == std::vector<cppush::Code>{code_list});
	REQUIRE(env.get_stack<cppush::Exec>().size() == 2);
	REQUIRE(*env.get_stack<cppush::Exec>()[0] == *pop_insn);
	REQUIRE(*env.get_stack<cppush::Exec>()[1] == *code_list);
}

TEST_CASE("Instruction: code_do*") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_do_star, "code_do*");

	std::shared_ptr<cppush::CodeList> code_list = std::make_shared<cppush::CodeList>();

	env.push<cppush::Code>(code_list);
	op(env);
	REQUIRE(env.get_stack<cppush::Code>().empty());
	REQUIRE(env.get_stack<cppush::Exec>().size() == 1);
	REQUIRE(*env.get_stack<cppush::Exec>()[0] == *code_list);
}

TEST_CASE("Instruction: code_quote") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_quote, "code_quote");

	std::shared_ptr<cppush::CodeList> code_list = std::make_shared<cppush::CodeList>();

	env.push<cppush::Exec>(code_list);
	op(env);
	REQUIRE(env.get_stack<cppush::Exec>().empty());
	REQUIRE(env.get_stack<cppush::Code>() == std::vector<cppush::Code>{code_list});
}

TEST_CASE("Instruction: code_do*range") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_do_range, "code_do*range");
	cppush::Instruction quote_op(cppush::code_quote, "code_quote");

	cppush::CodeList body;
	env.push<cppush::Code>(std::make_shared<cppush::CodeList>(body));

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
		REQUIRE(rcall.size() == 5);
		REQUIRE(*rcall[0] == cppush::Literal<int>(1));
		REQUIRE(*rcall[1] == cppush::Literal<int>(9));
		REQUIRE(*rcall[2] == quote_op);
		REQUIRE(*rcall[3] == body);
		REQUIRE(*rcall[4] == op);
		REQUIRE(*env.get_stack<cppush::Exec>()[1] == body);
	}

	SECTION("index > dest") {
		env.get_stack<int>().insert(env.get_stack<int>().end(), {9, 0});
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{9});
		REQUIRE(env.get_stack<cppush::Exec>().size() == 2);
		auto& rcall = env.get_stack<cppush::Exec>()[0]->get_stack();
		REQUIRE(rcall.size() == 5);
		REQUIRE(*rcall[0] == cppush::Literal<int>(8));
		REQUIRE(*rcall[1] == cppush::Literal<int>(0));
		REQUIRE(*rcall[2] == quote_op);
		REQUIRE(*rcall[3] == body);
		REQUIRE(*rcall[4] == op);
		REQUIRE(*env.get_stack<cppush::Exec>()[1] == body);
	}
}

TEST_CASE("Instruction: code_do*count") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_do_count, "code_do*count");
	cppush::Instruction do_range(cppush::code_do_range, "code_do*range");
	cppush::Instruction quote_op(cppush::code_quote, "code_quote");

	cppush::CodeList body;
	env.push<cppush::Code>(std::make_shared<cppush::CodeList>(body));

	int count;

	SECTION("count < 1") {
		count = 0;
		env.push<int>(count);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{count});
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == body);
	}

	SECTION("count > 0") {
		count = 2;
		env.push<int>(count);
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.get_stack<cppush::Exec>().size() == 1);
		auto& rcall = env.get_stack<cppush::Exec>().back()->get_stack();
		REQUIRE(rcall.size() == 5);
		REQUIRE(*rcall[0] == cppush::Literal<int>(0));
		REQUIRE(*rcall[1] == cppush::Literal<int>(count - 1));
		REQUIRE(*rcall[2] == quote_op);
		REQUIRE(*rcall[3] == body);
		REQUIRE(*rcall[4] == do_range);
	}
}

TEST_CASE("Instruction: code_do*times") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_do_times, "code_do*times");
	cppush::Instruction quote_op(cppush::code_quote, "code_quote");
	cppush::Instruction pop_insn(cppush::protected_pop<int>, "integer_pop");
	cppush::Instruction do_range(cppush::code_do_range, "code_do*range");

	cppush::CodeList body;
	env.push<cppush::Code>(std::make_shared<cppush::CodeList>(body));

	int times;

	SECTION("times < 1") {
		times = 0;
		env.push<int>(times);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{times});
		REQUIRE(env.get_stack<cppush::Exec>().empty());
		REQUIRE(*env.get_stack<cppush::Code>().back() == body);
	}

	SECTION("times > 0") {
		times = 2;
		env.push<int>(times);
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.get_stack<cppush::Exec>().size() == 1);
		auto& rcall = env.get_stack<cppush::Exec>().back()->get_stack();
		REQUIRE(rcall.size() == 5);
		REQUIRE(*rcall[0] == cppush::Literal<int>(0));
		REQUIRE(*rcall[1] == cppush::Literal<int>(times - 1));
		REQUIRE(*rcall[2] == quote_op);
		auto& pop_code = rcall[3]->get_stack();
		REQUIRE(pop_code.size() == 2);
		REQUIRE(*pop_code[0] == pop_insn);
		REQUIRE(*pop_code[1] == body);
		REQUIRE(*rcall[4] == do_range);
	}
}

TEST_CASE("Instruction: code_extract") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_extract, "code_extract");

	/*
	 * 0: ( B C ( A ) ( D ( A ) ) )
	 * 1: B
	 * 2: C
	 * 3: ( A )
	 * 4: A
	 * 5: ( D ( A ) )
	 * 6: D
	 * 7: ( A )
	 * 8: A
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);
	auto d = std::make_shared<cppush::Literal<int>>(3);

	std::vector<cppush::Code> list_a{a};
	std::vector<cppush::Code> list_da{d, std::make_shared<cppush::CodeList>(list_a)};

	std::vector<cppush::Code> list_full{
		b,
		c,
		std::make_shared<cppush::CodeList>(list_a),
		std::make_shared<cppush::CodeList>(list_da)
	};

	std::vector<cppush::Code> indexed_list{
		std::make_shared<cppush::CodeList>(list_full),
		b,
		c,
		list_full[2],
		a,
		list_full[3],
		d,
		list_full[2],
		a
	};

	int index_upper = indexed_list.size();

	auto container = std::make_shared<cppush::CodeList>(list_full);

	SECTION("Positive indexes") {
		for (int i = 0; i < index_upper; ++i) {
			env.get_stack<cppush::Code>().clear();
			env.get_stack<int>().clear();
			env.push<cppush::Code>(container);
			env.push<int>(i);
			op(env);
			REQUIRE(env.get_stack<int>().empty());
			REQUIRE(env.get_stack<cppush::Code>().size() == 1);
			REQUIRE(*env.get_stack<cppush::Code>().back() == *indexed_list[i]);
		}
	}

	SECTION("Positive indexes out of bounds") {
		for (int i = 0; i < index_upper; ++i) {
			env.get_stack<cppush::Code>().clear();
			env.get_stack<int>().clear();
			env.push<cppush::Code>(container);
			env.push<int>(i + index_upper);
			op(env);
			REQUIRE(env.get_stack<int>().empty());
			REQUIRE(env.get_stack<cppush::Code>().size() == 1);
			REQUIRE(*env.get_stack<cppush::Code>().back() == *indexed_list[i]);
		}
	}

	SECTION("Negative indexes") {
		for (int i = -1; i > -index_upper; --i) {
			env.get_stack<cppush::Code>().clear();
			env.get_stack<int>().clear();
			env.push<cppush::Code>(container);
			env.push<int>(i);
			op(env);
			REQUIRE(env.get_stack<int>().empty());
			REQUIRE(env.get_stack<cppush::Code>().size() == 1);
			REQUIRE(*env.get_stack<cppush::Code>().back() == *indexed_list[std::abs(i)]);
		}
	}

	SECTION("Negative indexes out of bounds") {
		for (int i = 0; i > -index_upper; --i) {
			env.get_stack<cppush::Code>().clear();
			env.get_stack<int>().clear();
			env.push<cppush::Code>(container);
			env.push<int>(i - index_upper);
			op(env);
			REQUIRE(env.get_stack<int>().empty());
			REQUIRE(env.get_stack<cppush::Code>().size() == 1);
			REQUIRE(*env.get_stack<cppush::Code>().back() == *indexed_list[std::abs(i)]);
		}
	}
}

TEST_CASE("Instruction: code_from_bool") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_from_bool, "code_from_bool");

	auto bool_literal = std::make_shared<cppush::Literal<bool>>(true);
	env.push<bool>(true);
	op(env);
	REQUIRE(env.get_stack<bool>().empty());
	REQUIRE(*env.get_stack<cppush::Code>().back() == *bool_literal);
}

TEST_CASE("Instruction: code_from_float") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_from_float, "code_from_float");

	const double val = 6.9; // nice

	auto float_literal = std::make_shared<cppush::Literal<double>>(val);
	env.push<double>(val);
	op(env);
	REQUIRE(env.get_stack<double>().empty());
	REQUIRE(*env.get_stack<cppush::Code>().back() == *float_literal);
}

TEST_CASE("Instruction: code_from_int") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_from_int, "code_from_int");

	const int val = 69; // nice

	auto int_literal = std::make_shared<cppush::Literal<int>>(val);
	env.push<int>(val);
	op(env);
	REQUIRE(env.get_stack<int>().empty());
	REQUIRE(*env.get_stack<cppush::Code>().back() == *int_literal);
}

TEST_CASE("Instruction: code_if") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_if, "code_if");
	cppush::CodeList branchA;
	cppush::CodeList branchB({std::make_shared<cppush::CodeList>(branchA)});

	env.push<cppush::Code>(std::make_shared<cppush::CodeList>(branchB));
	env.push<cppush::Code>(std::make_shared<cppush::CodeList>(branchA));

	REQUIRE(!(branchA == branchB));

	SECTION("true") {
		env.push<bool>(true);
		op(env);
		REQUIRE(env.get_stack<bool>().empty());
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.get_stack<cppush::Exec>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Exec>().back() == branchB);
	}

	SECTION("false") {
		env.push<bool>(false);
		op(env);
		REQUIRE(env.get_stack<bool>().empty());
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.get_stack<cppush::Exec>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Exec>().back() == branchA);
	}
}

TEST_CASE("Instruction: code_insert") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_insert, "code_insert");

	/*
	 * 0: ( B C ( A ) )
	 * 1: B
	 * 2: C
	 * 3: ( A )
	 * 4: A
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);
	auto d = std::make_shared<cppush::Literal<int>>(3);

	std::vector<cppush::Code> list_a{a};
	auto list_a_ptr = std::make_shared<cppush::CodeList>(list_a);

	std::vector<cppush::Code> list_full{
		b,
		c,
		std::make_shared<cppush::CodeList>(list_a),
	};

	auto container = std::make_shared<cppush::CodeList>(list_full);

	SECTION("Positive index") {
		env.push<cppush::Code>(d);
		env.push<cppush::Code>(container);
		env.push<int>(4);
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		auto last = env.get_stack<cppush::Code>()[0]->get_stack().back()->get_stack()[0];
		REQUIRE(*last == *d);
	}

	SECTION("Positive index out of bounds") {
		env.push<cppush::Code>(d);
		env.push<cppush::Code>(container);
		env.push<int>(4 + container->size());
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		auto last = env.get_stack<cppush::Code>()[0]->get_stack().back()->get_stack()[0];
		REQUIRE(*last == *d);
	}

	SECTION("Negative index") {
		env.push<cppush::Code>(d);
		env.push<cppush::Code>(container);
		env.push<int>(-4);
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		auto last = env.get_stack<cppush::Code>()[0]->get_stack().back()->get_stack()[0];
		REQUIRE(*last == *d);
	}

	SECTION("Negative index out of bounds") {
		env.push<cppush::Code>(d);
		env.push<cppush::Code>(container);
		env.push<int>(-4 - container->size());
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		auto last = env.get_stack<cppush::Code>()[0]->get_stack().back()->get_stack()[0];
		REQUIRE(*last == *d);
	}
}

TEST_CASE("Instruction: code_length") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_length, "code_length");

	/*
	 * ( B C ( A ) )
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);
	auto d = std::make_shared<cppush::Literal<int>>(3);

	std::vector<cppush::Code> list_a{a};
	auto list_a_ptr = std::make_shared<cppush::CodeList>(list_a);

	std::vector<cppush::Code> list_full{
		b,
		c,
		std::make_shared<cppush::CodeList>(list_a),
	};

	auto container = std::make_shared<cppush::CodeList>(list_full);

	env.push<cppush::Code>(container);
	op(env);
	REQUIRE(env.get_stack<cppush::Code>().empty());
	REQUIRE(env.get_stack<int>() == std::vector<int>{static_cast<int>(container->get_stack().size())});
}

TEST_CASE("Instruction: code_list") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_list, "code_list");

	/*
	 * A B -> ( B A )
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);

	env.push<cppush::Code>(b);
	env.push<cppush::Code>(a);
	op(env);
	REQUIRE(env.get_stack<cppush::Code>().size() == 1);
	auto& stack = env.get_stack<cppush::Code>()[0]->get_stack();
	REQUIRE(*stack[0] == *b);
	REQUIRE(*stack[1] == *a);
}

TEST_CASE("Instruction: code_member") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_member, "code_member");

	/*
	 * ( A B )
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);

	auto ab = std::make_shared<cppush::CodeList>(std::vector<cppush::Code>{a, b});

	SECTION("( A B ) A") {
		env.push<cppush::Code>(a);
		env.push<cppush::Code>(ab);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.pop<bool>() == true);
		REQUIRE(env.get_stack<bool>().empty());
	}

	SECTION("( A B ) C") {
		env.push<cppush::Code>(c);
		env.push<cppush::Code>(ab);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.pop<bool>() == false);
		REQUIRE(env.get_stack<bool>().empty());
	}
}

TEST_CASE("Instruction: code_noop") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_noop, "code_noop");

	env.push<cppush::Exec>(std::make_shared<cppush::Instruction>(op));
	auto exec_stack_copy = env.get_stack<cppush::Exec>();
	op(env);
	REQUIRE(env.get_stack<cppush::Exec>() == exec_stack_copy);
}

TEST_CASE("Instruction: code_nth") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_nth, "code_nth");

	/*
	 * ( B C ( A ) )
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);

	std::vector<cppush::Code> list_a{a};
	auto list_a_ptr = std::make_shared<cppush::CodeList>(list_a);

	std::vector<cppush::Code> list_full{
		b,
		c,
		list_a_ptr
	};

	auto container = std::make_shared<cppush::CodeList>(list_full);

	SECTION("Empty list") {
		auto empty_list = cppush::CodeList();
		env.push<cppush::Code>(std::make_shared<cppush::CodeList>(empty_list));
		env.push<int>(0);
		op(env);

		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == empty_list);
	}

	SECTION("Atom") {
		env.push<cppush::Code>(a);
		env.push<int>(0);
		op(env);

		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == *a);
	}

	SECTION("Nth") {
		env.push<cppush::Code>(container);
		env.push<int>(2);
		op(env);

		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == *list_a_ptr);
	}

	SECTION("Nth out of bounds") {
		env.push<cppush::Code>(container);
		env.push<int>(5);
		op(env);

		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == *list_a_ptr);
	}

	SECTION("Nth negative") {
		env.push<cppush::Code>(container);
		env.push<int>(-2);
		op(env);

		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == *list_a_ptr);
	}

	SECTION("Nth negative out of bounds") {
		env.push<cppush::Code>(container);
		env.push<int>(-5);
		op(env);

		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == *list_a_ptr);
	}
}

TEST_CASE("Instruction: code_nthcdr") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_nthcdr, "code_nthcdr");

	/*
	 * ( B C ( A ) )
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);

	std::vector<cppush::Code> list_a{a};
	auto list_a_ptr = std::make_shared<cppush::CodeList>(list_a);

	std::vector<cppush::Code> list_full{
		b,
		c,
		list_a_ptr
	};

	auto container = std::make_shared<cppush::CodeList>(list_full);

	SECTION("Empty list") {
		auto empty_list = cppush::CodeList();
		env.push<cppush::Code>(std::make_shared<cppush::CodeList>(empty_list));
		env.push<int>(1);
		op(env);

		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == empty_list);
	}

	SECTION("Atom") {
		env.push<cppush::Code>(a);
		env.push<int>(0);
		op(env);

		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == *a);
	}

	SECTION("Nth") {
		env.push<cppush::Code>(container);
		env.push<int>(1);
		op(env);

		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		auto stack = env.get_stack<cppush::Code>().back()->get_stack();
		for (int i = 0; i < static_cast<int>(stack.size()); ++i) {
			REQUIRE(*stack[i] == *list_full[i+1]);
		}
	}

	SECTION("Nth out of bounds") {
		env.push<cppush::Code>(container);
		env.push<int>(4);
		op(env);

		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		auto stack = env.get_stack<cppush::Code>().back()->get_stack();
		for (int i = 0; i < static_cast<int>(stack.size()); ++i) {
			REQUIRE(*stack[i] == *list_full[i+1]);
		}
	}

	SECTION("Nth negative") {
		env.push<cppush::Code>(container);
		env.push<int>(-1);
		op(env);

		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		auto stack = env.get_stack<cppush::Code>().back()->get_stack();
		for (int i = 0; i < static_cast<int>(stack.size()); ++i) {
			REQUIRE(*stack[i] == *list_full[i+1]);
		}
	}

	SECTION("Nth negative out of bounds") {
		env.push<cppush::Code>(container);
		env.push<int>(-4);
		op(env);

		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		auto stack = env.get_stack<cppush::Code>().back()->get_stack();
		for (int i = 0; i < static_cast<int>(stack.size()); ++i) {
			REQUIRE(*stack[i] == *list_full[i+1]);
		}
	}
}

TEST_CASE("Instruction: code_null") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_null, "code_null");

	auto nil = std::make_shared<cppush::CodeList>();
	auto a = std::make_shared<cppush::Literal<int>>(1);

	SECTION("Empty list") {
		env.push<cppush::Code>(nil);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.pop<bool>() == true);
		REQUIRE(env.get_stack<bool>().empty());
	}

	SECTION("Not an empty list") {
		env.push<cppush::Code>(a);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.pop<bool>() == false);
		REQUIRE(env.get_stack<bool>().empty());
	}
}

TEST_CASE("Instruction: code_position") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_position, "code_position");

	auto a = std::make_shared<cppush::Literal<int>>(1);
	auto b = std::make_shared<cppush::Literal<int>>(2);
	auto c = std::make_shared<cppush::Literal<int>>(3);
	auto ab = std::make_shared<cppush::CodeList>(std::vector<cppush::Code>{a, b});

	SECTION("Coerce first item to list") {
		env.push<cppush::Code>(a);
		env.push<cppush::Code>(a);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.get_stack<int>() == std::vector<int>{0});
	}

	SECTION("-1 when not found") {
		env.push<cppush::Code>(c);
		env.push<cppush::Code>(ab);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.get_stack<int>() == std::vector<int>{-1});
	}

	SECTION("Position found") {
		env.push<cppush::Code>(b);
		env.push<cppush::Code>(ab);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.get_stack<int>() == std::vector<int>{1});
	}
}

TEST_CASE("Instruction: code_size") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_size, "code_size");

	/*
	 * () = 1
	 * A = 1
	 * ( A A ) = 3
	 * ( A ( A A ) ) = 5
	 */

	auto nil = std::make_shared<cppush::CodeList>();
	auto a = std::make_shared<cppush::Literal<int>>(1);
	auto aa = std::make_shared<cppush::CodeList>(std::vector<cppush::Code>{a, a});
	auto nested_aa = std::make_shared<cppush::CodeList>(std::vector<cppush::Code>{a, aa});

	SECTION("() = 1") {
		env.push<cppush::Code>(nil);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.get_stack<int>() == std::vector<int>{1});
	}

	SECTION("A = 1") {
		env.push<cppush::Code>(a);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.get_stack<int>() == std::vector<int>{1});
	}

	SECTION("( A A ) = 3") {
		env.push<cppush::Code>(aa);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.get_stack<int>() == std::vector<int>{3});
	}

	SECTION("( A ( A A ) ) = 5") {
		env.push<cppush::Code>(nested_aa);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().empty());
		REQUIRE(env.get_stack<int>() == std::vector<int>{5});
	}
}

TEST_CASE("Instruction: code_subst") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_subst, "code_subst");

	/*
	 * A B B = A
	 * A B A = B
	 * A B C = A
	 * ( A B ) C B = ( A C )
	 */

	auto a = std::make_shared<cppush::Literal<int>>(1);
	auto b = std::make_shared<cppush::Literal<int>>(2);
	auto c = std::make_shared<cppush::Literal<int>>(3);
	auto ab = std::make_shared<cppush::CodeList>(std::vector<cppush::Code>{a, b});
	auto ac = std::make_shared<cppush::CodeList>(std::vector<cppush::Code>{a, c});

	SECTION("A B B = A") {
		env.push<cppush::Code>(b);
		env.push<cppush::Code>(b);
		env.push<cppush::Code>(a);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == *a);
	}

	SECTION("A B A = B") {
		env.push<cppush::Code>(a);
		env.push<cppush::Code>(b);
		env.push<cppush::Code>(a);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == *b);
	}

	SECTION("A B C = A") {
		env.push<cppush::Code>(c);
		env.push<cppush::Code>(b);
		env.push<cppush::Code>(a);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == *a);
	}

	SECTION("( A B ) C B = ( A C )") {
		env.push<cppush::Code>(b);
		env.push<cppush::Code>(c);
		env.push<cppush::Code>(ab);
		op(env);
		REQUIRE(env.get_stack<cppush::Code>().size() == 1);
		REQUIRE(*env.get_stack<cppush::Code>().back() == *ac);
	}
}
