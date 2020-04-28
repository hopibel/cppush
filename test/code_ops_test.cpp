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

TEST_CASE("Instruction: CODE.APPEND") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_append, "CODE.APPEND");

	SECTION("Both atoms"){
		auto first = std::make_shared<cppush::Literal<int>>(1);
		auto second = std::make_shared<cppush::Literal<int>>(1);
		env.code_stack.insert(env.code_stack.end(), {second, first});
		op(env);
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(env.code_stack.back()->is_list());
		auto& stack = env.code_stack.back()->get_stack();
		REQUIRE(stack[0] == second);
		REQUIRE(stack[1] == first);
	}

	SECTION("Both lists"){
		auto first = std::make_shared<cppush::CodeList>(
				std::vector<std::shared_ptr<cppush::Code>>{
					std::make_shared<cppush::Literal<int>>(1)
				});
		auto second = std::make_shared<cppush::CodeList>(
				std::vector<std::shared_ptr<cppush::Code>>{
					std::make_shared<cppush::Literal<int>>(2)
				});

		env.code_stack.insert(env.code_stack.end(), {second, first});
		op(env);
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(env.code_stack.back()->is_list());
		auto& stack = env.code_stack.back()->get_stack();
		REQUIRE(stack[0] == second->get_stack()[0]);
		REQUIRE(stack[1] == first->get_stack()[0]);
	}

	SECTION("Atom and list"){
		auto code_list = std::make_shared<cppush::CodeList>(
				std::vector<std::shared_ptr<cppush::Code>>{
					std::make_shared<cppush::Literal<int>>(1)
				});
		auto atom = std::make_shared<cppush::Literal<int>>(2);

		SECTION("Append list to atom") {
			env.code_stack.insert(env.code_stack.end(), {atom, code_list});
			op(env);
			REQUIRE(env.code_stack.size() == 1);
			REQUIRE(env.code_stack.back()->is_list());
			auto& stack = env.code_stack.back()->get_stack();
			REQUIRE(stack[0] == atom);
			REQUIRE(stack[1] == code_list->get_stack()[0]);
		}

		SECTION("Append atom to list") {
			env.code_stack.insert(env.code_stack.end(), {code_list, atom});
			op(env);
			REQUIRE(env.code_stack.size() == 1);
			REQUIRE(env.code_stack.back()->is_list());
			auto& stack = env.code_stack.back()->get_stack();
			REQUIRE(stack[0] == code_list->get_stack()[0]);
			REQUIRE(stack[1] == atom);
		}
	}
}

TEST_CASE("Instruction: CODE.ATOM") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_atom, "CODE.ATOM");

	SECTION("Atom") {
		auto atom = std::make_shared<cppush::Literal<int>>(0);
		env.code_stack.push_back(atom);
		op(env);
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("List") {
		auto code_list = std::make_shared<cppush::CodeList>();
		env.code_stack.push_back(code_list);
		op(env);
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}
}

TEST_CASE("Instruction: CODE.CAR") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_car, "CODE.CAR");

	SECTION("Atom") {
		auto atom = std::make_shared<cppush::Literal<int>>(0);
		env.code_stack.push_back(atom);
		op(env);
		REQUIRE(env.code_stack == std::vector<std::shared_ptr<cppush::Code>>{atom});
	}

	SECTION("List") {
		auto atom_a = std::make_shared<cppush::Literal<int>>(0);
		auto atom_b = std::make_shared<cppush::Literal<int>>(1);
		auto list_ab = std::vector<std::shared_ptr<cppush::Code>>{atom_a, atom_b};
		auto code_list = std::make_shared<cppush::CodeList>(list_ab);
		env.code_stack.push_back(code_list);
		op(env);
		REQUIRE(env.code_stack == std::vector<std::shared_ptr<cppush::Code>>{atom_a});
	}
}

TEST_CASE("Instruction: CODE.CDR") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_cdr, "CODE.CDR");

	SECTION("Atom") {
		auto atom = std::make_shared<cppush::Literal<int>>(0);
		env.code_stack.push_back(atom);
		op(env);
		// CDR on atom results in empty list
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == cppush::CodeList());
	}

	SECTION("List") {
		auto atom_a = std::make_shared<cppush::Literal<int>>(0);
		auto atom_b = std::make_shared<cppush::Literal<int>>(1);
		auto list_ab = std::vector<std::shared_ptr<cppush::Code>>{atom_a, atom_b};
		auto list_b = std::vector<std::shared_ptr<cppush::Code>>{atom_b};
		auto code_list = std::make_shared<cppush::CodeList>(list_ab);
		env.code_stack.push_back(code_list);
		op(env);
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == cppush::CodeList(list_b));
	}
}

TEST_CASE("Instruction: CODE.CONS") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_cons, "CODE.CONS");

	auto second = std::make_shared<cppush::Literal<int>>(1);

	SECTION("First item is an atom") {
		auto first = std::make_shared<cppush::Literal<int>>(0);
		env.code_stack.push_back(second);
		env.code_stack.push_back(first);
		op(env);
		REQUIRE(env.code_stack.size() == 1);
		auto& code_list = env.code_stack.back()->get_stack();
		REQUIRE(code_list.size() == 2);
		REQUIRE(*code_list[0] == *second);
		REQUIRE(*code_list[1] == *first);
	}

	SECTION("First item is a list") {
		auto first = std::make_shared<cppush::CodeList>(
				std::vector<std::shared_ptr<cppush::Code>>{std::make_shared<cppush::Literal<int>>(0)}
				);
		env.code_stack.push_back(second);
		env.code_stack.push_back(first);
		op(env);
		REQUIRE(env.code_stack.size() == 1);
		auto& code_list = env.code_stack.back()->get_stack();
		REQUIRE(code_list.size() == 2);
		REQUIRE(*code_list[0] == *second);
		REQUIRE(*code_list[1] == *first->get_stack()[0]);
	}
}

TEST_CASE("Instruction: CODE.CONTAINER") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_container, "CODE.CONTAINER");

	/*
	For example, if the top piece of code is "( B ( C ( A ) ) ( D ( A ) ) )" and the second piece of code is
	"( A )" then this pushes ( C ( A ) ). Pushes an empty list if there is no such container. 
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);
	auto d = std::make_shared<cppush::Literal<int>>(3);

	std::vector<std::shared_ptr<cppush::Code>> list_a{a};
	std::vector<std::shared_ptr<cppush::Code>> list_ca{c, std::make_shared<cppush::CodeList>(list_a)};
	std::vector<std::shared_ptr<cppush::Code>> list_bca{b, std::make_shared<cppush::CodeList>(list_ca)};
	std::vector<std::shared_ptr<cppush::Code>> list_da{d, std::make_shared<cppush::CodeList>(list_a)};

	std::vector<std::shared_ptr<cppush::Code>> list_full{
			std::make_shared<cppush::CodeList>(list_bca),
			std::make_shared<cppush::CodeList>(list_da)
	};

	auto container = std::make_shared<cppush::CodeList>(list_full);

	SECTION("Find container") {
		env.code_stack.push_back(std::make_shared<cppush::CodeList>(list_a));
		env.code_stack.push_back(container);
		op(env);

		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == cppush::CodeList(list_ca));
	}

	SECTION("Push empty list if no container") {
		env.code_stack.push_back(container);
		env.code_stack.push_back(std::make_shared<cppush::CodeList>(list_a));
		op(env);

		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == cppush::CodeList());
	}
}

TEST_CASE("Instruction: CODE.CONTAINS") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_contains, "CODE.CONTAINS");

	/*
	 * First:	( A )
	 * Second:	( B ( C ( A ) ) ( D ( A ) ) )
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);
	auto d = std::make_shared<cppush::Literal<int>>(3);

	std::vector<std::shared_ptr<cppush::Code>> list_a{a};
	std::vector<std::shared_ptr<cppush::Code>> list_ca{c, std::make_shared<cppush::CodeList>(list_a)};
	std::vector<std::shared_ptr<cppush::Code>> list_bca{b, std::make_shared<cppush::CodeList>(list_ca)};
	std::vector<std::shared_ptr<cppush::Code>> list_da{d, std::make_shared<cppush::CodeList>(list_a)};

	std::vector<std::shared_ptr<cppush::Code>> list_full{
			std::make_shared<cppush::CodeList>(list_bca),
			std::make_shared<cppush::CodeList>(list_da)
	};

	auto container = std::make_shared<cppush::CodeList>(list_full);

	SECTION("Subtree found") {
		env.code_stack.push_back(container);
		env.code_stack.push_back(std::make_shared<cppush::CodeList>(list_a));
		op(env);

		REQUIRE(env.code_stack.empty());
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("Not a subtree") {
		// swap, so we have a superset instead of a subset
		env.code_stack.push_back(std::make_shared<cppush::CodeList>(list_a));
		env.code_stack.push_back(container);
		op(env);

		REQUIRE(env.code_stack.empty());
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}

	SECTION("Items are equal") {
		env.code_stack.push_back(container);
		env.code_stack.push_back(container);
		op(env);

		REQUIRE(env.code_stack.empty());
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}
}

TEST_CASE("Instruction: CODE.DO") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_do, "CODE.DO");

	std::shared_ptr<cppush::Instruction> pop_insn = std::make_shared<cppush::Instruction>(
			cppush::protected_pop<cppush::Code_ptr>, "CODE.POP");
	std::shared_ptr<cppush::CodeList> code_list = std::make_shared<cppush::CodeList>();

	env.code_stack.push_back(code_list);
	op(env);
	REQUIRE(env.code_stack == std::vector<std::shared_ptr<cppush::Code>>{code_list});
	REQUIRE(env.exec_stack.size() == 2);
	REQUIRE(*env.exec_stack[0] == *pop_insn);
	REQUIRE(*env.exec_stack[1] == *code_list);
}

TEST_CASE("Instruction: CODE.DO*") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_do_star, "CODE.DO*");

	std::shared_ptr<cppush::CodeList> code_list = std::make_shared<cppush::CodeList>();

	env.code_stack.push_back(code_list);
	op(env);
	REQUIRE(env.code_stack.empty());
	REQUIRE(env.exec_stack.size() == 1);
	REQUIRE(*env.exec_stack[0] == *code_list);
}

TEST_CASE("Instruction: CODE.QUOTE") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_quote, "CODE.QUOTE");

	std::shared_ptr<cppush::CodeList> code_list = std::make_shared<cppush::CodeList>();

	env.exec_stack.push_back(code_list);
	op(env);
	REQUIRE(env.exec_stack.empty());
	REQUIRE(env.code_stack == std::vector<std::shared_ptr<cppush::Code>>{code_list});
}

TEST_CASE("Instruction: CODE.DO*RANGE") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_do_range, "CODE.DO*RANGE");
	cppush::Instruction quote_op(cppush::code_quote, "CODE.QUOTE");

	cppush::CodeList body;
	env.code_stack.push_back(std::make_shared<cppush::CodeList>(body));

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
		REQUIRE(rcall.size() == 5);
		REQUIRE(*rcall[0] == cppush::Literal<int>(1));
		REQUIRE(*rcall[1] == cppush::Literal<int>(9));
		REQUIRE(*rcall[2] == quote_op);
		REQUIRE(*rcall[3] == body);
		REQUIRE(*rcall[4] == op);
		REQUIRE(*env.exec_stack[1] == body);
	}

	SECTION("index > dest") {
		env.int_stack.insert(env.int_stack.end(), {9, 0});
		op(env);
		REQUIRE(env.int_stack == std::vector<int>{9});
		REQUIRE(env.exec_stack.size() == 2);
		auto& rcall = env.exec_stack[0]->get_stack();
		REQUIRE(rcall.size() == 5);
		REQUIRE(*rcall[0] == cppush::Literal<int>(8));
		REQUIRE(*rcall[1] == cppush::Literal<int>(0));
		REQUIRE(*rcall[2] == quote_op);
		REQUIRE(*rcall[3] == body);
		REQUIRE(*rcall[4] == op);
		REQUIRE(*env.exec_stack[1] == body);
	}
}

TEST_CASE("Instruction: CODE.DO*COUNT") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_do_count, "CODE.DO*COUNT");
	cppush::Instruction do_range(cppush::code_do_range, "CODE.DO*RANGE");
	cppush::Instruction quote_op(cppush::code_quote, "CODE.QUOTE");

	cppush::CodeList body;
	env.code_stack.push_back(std::make_shared<cppush::CodeList>(body));

	int count;

	SECTION("count < 1") {
		count = 0;
		env.int_stack.push_back(count);
		op(env);
		REQUIRE(env.int_stack == std::vector<int>{count});
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == body);
	}

	SECTION("count > 0") {
		count = 2;
		env.int_stack.push_back(count);
		op(env);
		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.exec_stack.size() == 1);
		auto& rcall = env.exec_stack.back()->get_stack();
		REQUIRE(rcall.size() == 5);
		REQUIRE(*rcall[0] == cppush::Literal<int>(0));
		REQUIRE(*rcall[1] == cppush::Literal<int>(count - 1));
		REQUIRE(*rcall[2] == quote_op);
		REQUIRE(*rcall[3] == body);
		REQUIRE(*rcall[4] == do_range);
	}
}

TEST_CASE("Instruction: CODE.DO*TIMES") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_do_times, "CODE.DO*TIMES");
	cppush::Instruction quote_op(cppush::code_quote, "CODE.QUOTE");
	cppush::Instruction pop_insn(cppush::protected_pop<int>, "INTEGER.POP");
	cppush::Instruction do_range(cppush::code_do_range, "CODE.DO*RANGE");

	cppush::CodeList body;
	env.code_stack.push_back(std::make_shared<cppush::CodeList>(body));

	int times;

	SECTION("times < 1") {
		times = 0;
		env.int_stack.push_back(times);
		op(env);
		REQUIRE(env.int_stack == std::vector<int>{times});
		REQUIRE(env.exec_stack.empty());
		REQUIRE(*env.code_stack.back() == body);
	}

	SECTION("times > 0") {
		times = 2;
		env.int_stack.push_back(times);
		op(env);
		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.exec_stack.size() == 1);
		auto& rcall = env.exec_stack.back()->get_stack();
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

TEST_CASE("Instruction: CODE.EXTRACT") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_extract, "CODE.EXTRACT");

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

	std::vector<std::shared_ptr<cppush::Code>> list_a{a};
	std::vector<std::shared_ptr<cppush::Code>> list_da{d, std::make_shared<cppush::CodeList>(list_a)};

	std::vector<std::shared_ptr<cppush::Code>> list_full{
		b,
		c,
		std::make_shared<cppush::CodeList>(list_a),
		std::make_shared<cppush::CodeList>(list_da)
	};

	std::vector<std::shared_ptr<cppush::Code>> indexed_list{
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
			env.code_stack.clear();
			env.int_stack.clear();
			env.code_stack.push_back(container);
			env.int_stack.push_back(i);
			op(env);
			REQUIRE(env.int_stack.empty());
			REQUIRE(env.code_stack.size() == 1);
			REQUIRE(*env.code_stack.back() == *indexed_list[i]);
		}
	}

	SECTION("Positive indexes out of bounds") {
		for (int i = 0; i < index_upper; ++i) {
			env.code_stack.clear();
			env.int_stack.clear();
			env.code_stack.push_back(container);
			env.int_stack.push_back(i + index_upper);
			op(env);
			REQUIRE(env.int_stack.empty());
			REQUIRE(env.code_stack.size() == 1);
			REQUIRE(*env.code_stack.back() == *indexed_list[i]);
		}
	}

	SECTION("Negative indexes") {
		for (int i = -1; i > -index_upper; --i) {
			env.code_stack.clear();
			env.int_stack.clear();
			env.code_stack.push_back(container);
			env.int_stack.push_back(i);
			op(env);
			REQUIRE(env.int_stack.empty());
			REQUIRE(env.code_stack.size() == 1);
			REQUIRE(*env.code_stack.back() == *indexed_list[std::abs(i)]);
		}
	}

	SECTION("Negative indexes out of bounds") {
		for (int i = 0; i > -index_upper; --i) {
			env.code_stack.clear();
			env.int_stack.clear();
			env.code_stack.push_back(container);
			env.int_stack.push_back(i - index_upper);
			op(env);
			REQUIRE(env.int_stack.empty());
			REQUIRE(env.code_stack.size() == 1);
			REQUIRE(*env.code_stack.back() == *indexed_list[std::abs(i)]);
		}
	}
}

TEST_CASE("Instruction: CODE.FROM_BOOL") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_from_bool, "CODE.FROM_BOOL");

	auto bool_literal = std::make_shared<cppush::Literal<bool>>(true);
	env.bool_stack.push_back(true);
	op(env);
	REQUIRE(env.bool_stack.empty());
	REQUIRE(*env.code_stack.back() == *bool_literal);
}

TEST_CASE("Instruction: CODE.FROM_FLOAT") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_from_float, "CODE.FROM_FLOAT");

	const double val = 6.9; // nice

	auto float_literal = std::make_shared<cppush::Literal<double>>(val);
	env.float_stack.push_back(val);
	op(env);
	REQUIRE(env.float_stack.empty());
	REQUIRE(*env.code_stack.back() == *float_literal);
}

TEST_CASE("Instruction: CODE.FROM_INT") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_from_int, "CODE.FROM_INT");

	const int val = 69; // nice

	auto int_literal = std::make_shared<cppush::Literal<int>>(val);
	env.int_stack.push_back(val);
	op(env);
	REQUIRE(env.int_stack.empty());
	REQUIRE(*env.code_stack.back() == *int_literal);
}

TEST_CASE("Instruction: CODE.IF") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_if, "CODE.IF");
	cppush::CodeList branchA;
	cppush::CodeList branchB({std::make_shared<cppush::CodeList>(branchA)});

	env.code_stack.push_back(std::make_shared<cppush::CodeList>(branchB));
	env.code_stack.push_back(std::make_shared<cppush::CodeList>(branchA));

	REQUIRE(!(branchA == branchB));

	SECTION("true") {
		env.bool_stack.push_back(true);
		op(env);
		REQUIRE(env.bool_stack.empty());
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.exec_stack.size() == 1);
		REQUIRE(*env.exec_stack.back() == branchB);
	}

	SECTION("false") {
		env.bool_stack.push_back(false);
		op(env);
		REQUIRE(env.bool_stack.empty());
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.exec_stack.size() == 1);
		REQUIRE(*env.exec_stack.back() == branchA);
	}
}

TEST_CASE("Instruction: CODE.INSERT") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_insert, "CODE.INSERT");

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

	std::vector<std::shared_ptr<cppush::Code>> list_a{a};
	auto list_a_ptr = std::make_shared<cppush::CodeList>(list_a);

	std::vector<std::shared_ptr<cppush::Code>> list_full{
		b,
		c,
		std::make_shared<cppush::CodeList>(list_a),
	};

	auto container = std::make_shared<cppush::CodeList>(list_full);

	SECTION("Positive index") {
		env.code_stack.push_back(d);
		env.code_stack.push_back(container);
		env.int_stack.push_back(4);
		op(env);
		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		auto last = env.code_stack[0]->get_stack().back()->get_stack()[0];
		REQUIRE(*last == *d);
	}

	SECTION("Positive index out of bounds") {
		env.code_stack.push_back(d);
		env.code_stack.push_back(container);
		env.int_stack.push_back(4 + container->size());
		op(env);
		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		auto last = env.code_stack[0]->get_stack().back()->get_stack()[0];
		REQUIRE(*last == *d);
	}

	SECTION("Negative index") {
		env.code_stack.push_back(d);
		env.code_stack.push_back(container);
		env.int_stack.push_back(-4);
		op(env);
		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		auto last = env.code_stack[0]->get_stack().back()->get_stack()[0];
		REQUIRE(*last == *d);
	}

	SECTION("Negative index out of bounds") {
		env.code_stack.push_back(d);
		env.code_stack.push_back(container);
		env.int_stack.push_back(-4 - container->size());
		op(env);
		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		auto last = env.code_stack[0]->get_stack().back()->get_stack()[0];
		REQUIRE(*last == *d);
	}
}

TEST_CASE("Instruction: CODE.LENGTH") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_length, "CODE.LENGTH");

	/*
	 * ( B C ( A ) )
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);
	auto d = std::make_shared<cppush::Literal<int>>(3);

	std::vector<std::shared_ptr<cppush::Code>> list_a{a};
	auto list_a_ptr = std::make_shared<cppush::CodeList>(list_a);

	std::vector<std::shared_ptr<cppush::Code>> list_full{
		b,
		c,
		std::make_shared<cppush::CodeList>(list_a),
	};

	auto container = std::make_shared<cppush::CodeList>(list_full);

	env.code_stack.push_back(container);
	op(env);
	REQUIRE(env.code_stack.empty());
	REQUIRE(env.int_stack == std::vector<int>{static_cast<int>(container->get_stack().size())});
}

TEST_CASE("Instruction: CODE.LIST") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_list, "CODE.LIST");

	/*
	 * A B -> ( B A )
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);

	env.code_stack.push_back(b);
	env.code_stack.push_back(a);
	op(env);
	REQUIRE(env.code_stack.size() == 1);
	auto& stack = env.code_stack[0]->get_stack();
	REQUIRE(*stack[0] == *b);
	REQUIRE(*stack[1] == *a);
}

TEST_CASE("Instruction: CODE.MEMBER") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_member, "CODE.MEMBER");

	/*
	 * ( A B )
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);

	auto ab = std::make_shared<cppush::CodeList>(std::vector<std::shared_ptr<cppush::Code>>{a, b});

	SECTION("( A B ) A") {
		env.code_stack.push_back(a);
		env.code_stack.push_back(ab);
		op(env);
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("( A B ) C") {
		env.code_stack.push_back(c);
		env.code_stack.push_back(ab);
		op(env);
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}
}

TEST_CASE("Instruction: CODE.NOOP") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_noop, "CODE.NOOP");

	env.exec_stack.push_back(std::make_shared<cppush::Instruction>(op));
	auto exec_stack_copy = env.exec_stack;
	op(env);
	REQUIRE(env.exec_stack == exec_stack_copy);
}

TEST_CASE("Instruction: CODE.NTH") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_nth, "CODE.NTH");

	/*
	 * ( B C ( A ) )
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);

	std::vector<std::shared_ptr<cppush::Code>> list_a{a};
	auto list_a_ptr = std::make_shared<cppush::CodeList>(list_a);

	std::vector<std::shared_ptr<cppush::Code>> list_full{
		b,
		c,
		list_a_ptr
	};

	auto container = std::make_shared<cppush::CodeList>(list_full);

	SECTION("Empty list") {
		auto empty_list = cppush::CodeList();
		env.code_stack.push_back(std::make_shared<cppush::CodeList>(empty_list));
		env.int_stack.push_back(0);
		op(env);

		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == empty_list);
	}

	SECTION("Atom") {
		env.code_stack.push_back(a);
		env.int_stack.push_back(0);
		op(env);

		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == *a);
	}

	SECTION("Nth") {
		env.code_stack.push_back(container);
		env.int_stack.push_back(2);
		op(env);

		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == *list_a_ptr);
	}

	SECTION("Nth out of bounds") {
		env.code_stack.push_back(container);
		env.int_stack.push_back(5);
		op(env);

		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == *list_a_ptr);
	}

	SECTION("Nth negative") {
		env.code_stack.push_back(container);
		env.int_stack.push_back(-2);
		op(env);

		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == *list_a_ptr);
	}

	SECTION("Nth negative out of bounds") {
		env.code_stack.push_back(container);
		env.int_stack.push_back(-5);
		op(env);

		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == *list_a_ptr);
	}
}

TEST_CASE("Instruction: CODE.NTHCDR") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_nthcdr, "CODE.NTHCDR");

	/*
	 * ( B C ( A ) )
	*/

	auto a = std::make_shared<cppush::Literal<int>>(0);
	auto b = std::make_shared<cppush::Literal<int>>(1);
	auto c = std::make_shared<cppush::Literal<int>>(2);

	std::vector<std::shared_ptr<cppush::Code>> list_a{a};
	auto list_a_ptr = std::make_shared<cppush::CodeList>(list_a);

	std::vector<std::shared_ptr<cppush::Code>> list_full{
		b,
		c,
		list_a_ptr
	};

	auto container = std::make_shared<cppush::CodeList>(list_full);

	SECTION("Empty list") {
		auto empty_list = cppush::CodeList();
		env.code_stack.push_back(std::make_shared<cppush::CodeList>(empty_list));
		env.int_stack.push_back(1);
		op(env);

		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == empty_list);
	}

	SECTION("Atom") {
		env.code_stack.push_back(a);
		env.int_stack.push_back(0);
		op(env);

		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == *a);
	}

	SECTION("Nth") {
		env.code_stack.push_back(container);
		env.int_stack.push_back(1);
		op(env);

		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		auto stack = env.code_stack.back()->get_stack();
		for (int i = 0; i < static_cast<int>(stack.size()); ++i) {
			REQUIRE(*stack[i] == *list_full[i+1]);
		}
	}

	SECTION("Nth out of bounds") {
		env.code_stack.push_back(container);
		env.int_stack.push_back(4);
		op(env);

		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		auto stack = env.code_stack.back()->get_stack();
		for (int i = 0; i < static_cast<int>(stack.size()); ++i) {
			REQUIRE(*stack[i] == *list_full[i+1]);
		}
	}

	SECTION("Nth negative") {
		env.code_stack.push_back(container);
		env.int_stack.push_back(-1);
		op(env);

		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		auto stack = env.code_stack.back()->get_stack();
		for (int i = 0; i < static_cast<int>(stack.size()); ++i) {
			REQUIRE(*stack[i] == *list_full[i+1]);
		}
	}

	SECTION("Nth negative out of bounds") {
		env.code_stack.push_back(container);
		env.int_stack.push_back(-4);
		op(env);

		REQUIRE(env.int_stack.empty());
		REQUIRE(env.code_stack.size() == 1);
		auto stack = env.code_stack.back()->get_stack();
		for (int i = 0; i < static_cast<int>(stack.size()); ++i) {
			REQUIRE(*stack[i] == *list_full[i+1]);
		}
	}
}

TEST_CASE("Instruction: CODE.NULL") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_null, "CODE.NULL");

	auto nil = std::make_shared<cppush::CodeList>();
	auto a = std::make_shared<cppush::Literal<int>>(1);

	SECTION("Empty list") {
		env.code_stack.push_back(nil);
		op(env);
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.bool_stack == std::vector<bool>{true});
	}

	SECTION("Not an empty list") {
		env.code_stack.push_back(a);
		op(env);
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.bool_stack == std::vector<bool>{false});
	}
}

TEST_CASE("Instruction: CODE.POSITION") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_position, "CODE.POSITION");

	auto a = std::make_shared<cppush::Literal<int>>(1);
	auto b = std::make_shared<cppush::Literal<int>>(2);
	auto c = std::make_shared<cppush::Literal<int>>(3);
	auto ab = std::make_shared<cppush::CodeList>(std::vector<std::shared_ptr<cppush::Code>>{a, b});

	SECTION("Coerce first item to list") {
		env.code_stack.push_back(a);
		env.code_stack.push_back(a);
		op(env);
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.int_stack == std::vector<int>{0});
	}

	SECTION("-1 when not found") {
		env.code_stack.push_back(c);
		env.code_stack.push_back(ab);
		op(env);
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.int_stack == std::vector<int>{-1});
	}

	SECTION("Position found") {
		env.code_stack.push_back(b);
		env.code_stack.push_back(ab);
		op(env);
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.int_stack == std::vector<int>{1});
	}
}

TEST_CASE("Instruction: CODE.SIZE") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_size, "CODE.SIZE");

	/*
	 * () = 1
	 * A = 1
	 * ( A A ) = 3
	 * ( A ( A A ) ) = 5
	 */

	auto nil = std::make_shared<cppush::CodeList>();
	auto a = std::make_shared<cppush::Literal<int>>(1);
	auto aa = std::make_shared<cppush::CodeList>(std::vector<std::shared_ptr<cppush::Code>>{a, a});
	auto nested_aa = std::make_shared<cppush::CodeList>(std::vector<std::shared_ptr<cppush::Code>>{a, aa});

	SECTION("() = 1") {
		env.code_stack.push_back(nil);
		op(env);
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.int_stack == std::vector<int>{1});
	}

	SECTION("A = 1") {
		env.code_stack.push_back(a);
		op(env);
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.int_stack == std::vector<int>{1});
	}

	SECTION("( A A ) = 3") {
		env.code_stack.push_back(aa);
		op(env);
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.int_stack == std::vector<int>{3});
	}

	SECTION("( A ( A A ) ) = 5") {
		env.code_stack.push_back(nested_aa);
		op(env);
		REQUIRE(env.code_stack.empty());
		REQUIRE(env.int_stack == std::vector<int>{5});
	}
}

TEST_CASE("Instruction: CODE.SUBST") {
	cppush::Env env;
	cppush::Instruction op(cppush::code_subst, "CODE.SUBST");

	/*
	 * A B B = A
	 * A B A = B
	 * A B C = A
	 * ( A B ) C B = ( A C )
	 */

	auto a = std::make_shared<cppush::Literal<int>>(1);
	auto b = std::make_shared<cppush::Literal<int>>(2);
	auto c = std::make_shared<cppush::Literal<int>>(3);
	auto ab = std::make_shared<cppush::CodeList>(std::vector<std::shared_ptr<cppush::Code>>{a, b});
	auto ac = std::make_shared<cppush::CodeList>(std::vector<std::shared_ptr<cppush::Code>>{a, c});

	SECTION("A B B = A") {
		env.code_stack.push_back(b);
		env.code_stack.push_back(b);
		env.code_stack.push_back(a);
		op(env);
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == *a);
	}

	SECTION("A B A = B") {
		env.code_stack.push_back(a);
		env.code_stack.push_back(b);
		env.code_stack.push_back(a);
		op(env);
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == *b);
	}

	SECTION("A B C = A") {
		env.code_stack.push_back(c);
		env.code_stack.push_back(b);
		env.code_stack.push_back(a);
		op(env);
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == *a);
	}

	SECTION("( A B ) C B = ( A C )") {
		env.code_stack.push_back(b);
		env.code_stack.push_back(c);
		env.code_stack.push_back(ab);
		op(env);
		REQUIRE(env.code_stack.size() == 1);
		REQUIRE(*env.code_stack.back() == *ac);
	}
}
