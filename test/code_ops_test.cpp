#include "code.h"
#include "test_utils.h"

#include <catch2/catch.hpp>

#include "code_ops.h"
#include "env.h"

#include <cmath>
#include <functional>
#include <vector>

#include <iostream>

using namespace cppush;

TEST_CASE("Instruction: code_append") {
	Env env;
	Instruction op(code_append, "code_append");
	auto vec = generate_test_values<Code>(3);

	SECTION("Both atoms or both lists") {
		Code expected = CodeList({vec[0], vec[1]});

		SECTION("1 0 -> (0 1)"){
			env.push<Code>(vec[0]);
			env.push<Code>(vec[1]);
			op(env);
			REQUIRE(env.pop<Code>() == expected);
			REQUIRE(env.get_stack<Code>().empty());
		}
		SECTION("(1)(0) -> (0 1)"){
			env.push<Code>(CodeList({ vec[0] }));
			env.push<Code>(CodeList({ vec[1] }));
			op(env);
			REQUIRE(env.pop<Code>() == expected);
			REQUIRE(env.get_stack<Code>().empty());
		}
	}
	SECTION("Atom and list"){
		auto code_list = CodeList({ vec[0], vec[1] });
		auto atom = vec[2];

		SECTION("Append list to atom: (0 1) 2 -> (2 0 1)") {
			env.push<Code>(atom);
			env.push<Code>(code_list);
			op(env);
			REQUIRE(env.pop<Code>() == Code(CodeList({atom, vec[0], vec[1]})));
			REQUIRE(env.get_stack<Code>().empty());
		}
		SECTION("Append atom to list: 2 (0 1) -> (0 1 2)") {
			env.push<Code>(code_list);
			env.push<Code>(atom);
			op(env);
			REQUIRE(env.pop<Code>() == Code(CodeList({vec[0], vec[1], atom})));
			REQUIRE(env.get_stack<Code>().empty());
		}
	}
}

TEST_CASE("Instruction: code_atom") {
	Env env;
	Instruction op(code_atom, "code_atom");
	bool expected;
	
	SECTION("Atom: 0 -> true") {
		env.push<Code>(Literal(0));
		expected = true;
	}

	SECTION("List: () -> false") {
		env.push<Code>(CodeList());
		expected = false;
	}

	op(env);
	REQUIRE(env.get_stack<Code>().empty());
	REQUIRE(env.pop<bool>() == expected);
}

TEST_CASE("Instruction: code_car") {
	Env env;
	Instruction op(code_car, "code_car");
	auto vec = generate_test_values<Code>(2);

	SECTION("Atom: 0 -> 0") {env.push<Code>(vec[0]);}
	SECTION("List: (0 1) -> 0") {env.push<Code>(CodeList({ vec[0], vec[1] }));}

	op(env);
	REQUIRE(env.get_stack<Code>() == std::vector<Code>{vec[0]});
}

TEST_CASE("Instruction: code_cdr") {
	Env env;
	Instruction op(code_cdr, "code_cdr");
	auto vec = generate_test_values<Code>(3);
	Code expected{ CodeList() };

	SECTION("Atom: 0 -> {}") {
		env.push<Code>(vec[0]);
		expected = CodeList();
	}

	SECTION("List: (0 1 2) -> (1 2)") {
		env.push<Code>(CodeList(vec));
		expected = CodeList({ vec[1], vec[2] });
	}

	op(env);
	REQUIRE(env.get_stack<Code>().size() == 1);
	REQUIRE(env.pop<Code>() == expected);
}

TEST_CASE("Instruction: code_cons") {
	Env env;
	Instruction op(code_cons, "code_cons");
	auto vec = generate_test_values<Code>(3);
	CodeList expected;

	env.push<Code>(vec[0]);
	SECTION("First item is an atom: 1 0 -> (0 1)") {
		env.push<Code>(vec[1]);
		expected = CodeList({ vec[0], vec[1] });
	}

	SECTION("First item is a list: (1 2) 0 -> (0 1 2)") {
		env.push<Code>(CodeList({ vec[1], vec[2] }));
		expected = CodeList(vec);
	}

	op(env);
	REQUIRE(env.get_stack<Code>().size() == 1);
	auto code_list = std::get<CodeList>(env.pop<Code>());
	REQUIRE(code_list == expected);
}

TEST_CASE("Instruction: code_container") {
	Env env;
	Instruction op(code_container, "code_container");
	auto vec = generate_test_values<Code>(4);
	CodeList expected;

	// (
	// 	( B ( C ( A ) ) )
	// 	( D ( A ) )
	// )
	CodeList container({
		CodeList({
			vec[1],
			CodeList({
				vec[2],
				CodeList({ vec[0] })
			})
		}),
		CodeList({
			vec[3],
			CodeList({ vec[0] })
		})
	});

	SECTION("Find container: ((B(C(A)))(D(A))) (A) -> (C(A))") {
		env.push<Code>(CodeList({ vec[0] }));
		env.push<Code>(container);
		expected = CodeList({ vec[2], CodeList({ vec[0] }) });
	}

	SECTION("Push empty list if no container: (A) ((B(C(A)))(D(A))) -> ()") {
		env.push<Code>(container);
		env.push<Code>(CodeList({ vec[0] }));
		expected = CodeList();
	}

	op(env);
	REQUIRE(env.get_stack<Code>().size() == 1);
	REQUIRE(env.pop<Code>() == Code(expected));
}

TEST_CASE("Instruction: code_contains") {
	Env env;
	Instruction op(code_contains, "code_contains");
	auto vec = generate_test_values<Code>(4);
	bool expected;

	CodeList container({
		CodeList({
			vec[1],
			CodeList({
				vec[2],
				CodeList({ vec[0] })
			})
		}),
		CodeList({
			vec[3],
			CodeList({ vec[0] })
		})
	});

	SECTION("Subtree found: (A) ((B(C(A)))(D(A))) -> true") {
		env.push<Code>(container);
		env.push<Code>(CodeList({ vec[0] }));
		expected = true;
	}

	SECTION("Not a subtree: ((B(C(A)))(D(A))) (A) -> false") {
		// swap, so we have a superset instead of a subset
		env.push<Code>(CodeList({ vec[0] }));
		env.push<Code>(container);
		expected = false;
	}

	SECTION("Items are equal: ((B(C(A)))(D(A))) ((B(C(A)))(D(A))) -> true") {
		// not strict subset
		env.push<Code>(container);
		env.push<Code>(container);
		expected = true;
	}

	op(env);
	REQUIRE(env.get_stack<Code>().empty());
	REQUIRE(env.pop<bool>() == expected);
}

TEST_CASE("Instruction: code_do\nexec: {} -> (body) code_pop\ncode: (body) -> (body)") {
	Env env;
	Instruction op(code_do, "code_do");
	auto pop_insn = Instruction(
		protected_pop<Code>, "code_pop");
	auto code_list = CodeList();

	env.push<Code>(code_list);
	op(env);
	REQUIRE(env.get_stack<Code>().size() == 1);
	REQUIRE(env.pop<Code>() == Code(code_list));
	REQUIRE(env.get_stack<Exec>().size() == 2);
	REQUIRE(env.pop<Exec>() == Code(code_list));
	REQUIRE(env.pop<Exec>() == Code(pop_insn));
}

TEST_CASE("Instruction: code_do*\nexec: {} -> (body)\ncode: (body) -> {}") {
	Env env;
	Instruction op(code_do_star, "code_do*");

	env.push<Code>(CodeList());
	op(env);
	REQUIRE(env.get_stack<Code>().empty());
	REQUIRE(env.get_stack<Exec>().size() == 1);
	REQUIRE(env.pop<Exec>() == Code(CodeList()));
}

TEST_CASE("Instruction: code_quote\nexec: (body) -> {}\ncode: {} -> (body)") {
	Env env;
	Instruction op(code_quote, "code_quote");

	env.push<Exec>(CodeList());
	op(env);
	REQUIRE(env.get_stack<Exec>().empty());
	REQUIRE(env.get_stack<Code>().size() == 1);
	REQUIRE(env.pop<Code>() == Code(CodeList()));
}

TEST_CASE("Instruction: code_do*range") {
	Env env;
	Instruction op(code_do_range, "code_do*range");
	Instruction quote_op(code_quote, "code_quote");
	std::vector<Code> expected_exec;
	int expected_int;

	CodeList body;
	env.push<Code>(body);

	SECTION("index == dest\n"
		"exec: {} -> (body)\n"
		"code: (body) -> {}\n"
		"int:  0 0 -> 0")
	{
		env.push<int>(0);
		env.push<int>(0);
		expected_int = 0;
		expected_exec = { body };
	}

	SECTION("index < dest\n"
		"exec: {} -> (body) (1 9 code_quote (body) code_do*range)\n"
		"code: (body) -> {}\n"
		"int:  9 0 -> 0")
	{
		env.push<int>(0);
		env.push<int>(9);
		expected_int = 0;
		expected_exec = { // recursive call
			CodeList({ Literal(1), Literal(9), quote_op, body, op }),
			body
		};
	}

	SECTION("index > dest\n"
		"exec: {} -> (body) (8 0 code_quote (body) code_do*range)\n"
		"code: (body) -> {}\n"
		"int:  0 9 -> 9")
	{
		env.push<int>(9);
		env.push<int>(0);
		expected_int = 9;
		expected_exec = { // recursive call
			CodeList({ Literal(8), Literal(0), quote_op, body, op }),
			body
		};
	}

	op(env);
	REQUIRE(env.get_stack<int>() == std::vector<int>{ expected_int });
	REQUIRE(env.get_stack<Exec>() == expected_exec);
}

TEST_CASE("Instruction: code_do*count") {
	Env env;
	Instruction op(code_do_count, "code_do*count");
	Instruction do_range(code_do_range, "code_do*range");
	Instruction quote_op(code_quote, "code_quote");
	
	CodeList body;
	env.push<Code>(body);

	int count;

	SECTION("count < 1 is noop") {
		count = 0;
		env.push<int>(count);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{count});
		REQUIRE(env.get_stack<Code>() == std::vector<Code>{body});
	}

	SECTION("count > 0\n"
		"exec: {} -> (0 1 code_quote (body) code_do*range)\n"
		"code: (body) -> {}\n"
		"int:  2 -> {}")
	{
		count = 2;
		env.push<int>(count);
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<Code>().empty());
		REQUIRE(env.get_stack<Exec>() == std::vector<Code>{CodeList({
			Literal(0), Literal(count - 1), quote_op, body, do_range
		})});
	}
}

TEST_CASE("Instruction: code_do*times") {
	Env env;
	Instruction op(code_do_times, "code_do*times");
	Instruction quote_op(code_quote, "code_quote");
	Instruction pop_insn(protected_pop<int>, "integer_pop");
	Instruction do_range(code_do_range, "code_do*range");

	CodeList body;
	env.push<Code>(body);

	int times;

	SECTION("times < 1 is noop") {
		times = 0;
		env.push<int>(times);
		op(env);
		REQUIRE(env.get_stack<int>() == std::vector<int>{times});
		REQUIRE(env.get_stack<Exec>().empty());
		REQUIRE(env.pop<Code>() == Code(body));
	}

	SECTION("times > 0\n"
		"exec: {} -> (0 1 code_quote (integer_pop (body)) code_do*range)\n"
		"code: (body) -> {}\n"
		"int:  2 -> {}")
	{
		times = 2;
		env.push<int>(times);
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<Code>().empty());
		REQUIRE(env.get_stack<Exec>().size() == 1);
		REQUIRE(env.pop<Exec>() == Code(CodeList({
			Literal(0), Literal(times - 1), quote_op,
			CodeList({ pop_insn, body }), do_range
		})));
	}
}

TEST_CASE("Instruction: code_extract") {
	Env env;
	Instruction op(code_extract, "code_extract");
	auto vec = generate_test_values<Code>(4);

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

	CodeList container({
		vec[1],
		vec[2],
		CodeList({vec[0]}),
		CodeList({
			vec[3],
			CodeList({ vec[0] })
		})
	});

	std::vector<Code> indexed{
		container,
		vec[1],
		vec[2],
		CodeList({ vec[0] }),
		vec[0],
		CodeList({ vec[3], CodeList({ vec[0] }) }),
		vec[3],
		CodeList({ vec[0] }),
		vec[0]
	};

	std::function<int(int)> get_index;
	SECTION("Positive indexes") {
		get_index = [&](int i) { return i; };
	}
	SECTION("Positive indexes out of bounds") {
		get_index = [&](int i) { return i + indexed.size(); };
	}
	SECTION("Negative indexes") {
		get_index = [&](int i) { return -i; };
	}
	SECTION("Negative indexes out of bounds") {
		get_index = [&](int i) { return -i - indexed.size(); };
	}
	for (int i = 0; i < static_cast<int>(indexed.size()); ++i) {
		env.push<Code>(container);

		env.push<int>(get_index(i));
		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<Code>().size() == 1);
		REQUIRE(env.pop<Code>() == indexed[i]);
	}
}

TEST_CASE("Instruction: code_from_bool") {
	Env env;
	Instruction op(code_from_bool, "code_from_bool");

	env.push<bool>(true);
	op(env);
	REQUIRE(env.get_stack<bool>().empty());
	REQUIRE(env.get_stack<Code>().size() == 1);
	REQUIRE(env.pop<Code>() == Code(Literal(true)));
}

TEST_CASE("Instruction: code_from_float") {
	Env env;
	Instruction op(code_from_float, "code_from_float");

	env.push<double>(4.2);
	op(env);
	REQUIRE(env.get_stack<double>().empty());
	REQUIRE(env.get_stack<Code>().size() == 1);
	REQUIRE(env.pop<Code>() == Code(Literal(4.2)));
}

TEST_CASE("Instruction: code_from_int") {
	Env env;
	Instruction op(code_from_int, "code_from_int");

	env.push<int>(42);
	op(env);
	REQUIRE(env.get_stack<int>().empty());
	REQUIRE(env.get_stack<Code>().size() == 1);
	REQUIRE(env.pop<Code>() == Code(Literal(42)));
}

TEST_CASE("Instruction: code_if") {
	Env env;
	Instruction op(code_if, "code_if");
	CodeList branchA;
	CodeList branchB({ Code(branchA) });
	CodeList expected;

	env.push<Code>(branchB);
	env.push<Code>(branchA);

	REQUIRE(branchA != branchB);

	SECTION("true\n"
		"exec: {} -> B\n"
		"code: A B -> {}\n"
		"bool: true -> {}")
	{
		env.push<bool>(true);
		expected = branchB;
	}

	SECTION("false\n"
		"exec: {} -> A\n"
		"code: A B -> {}\n"
		"bool: F -> {}")
	{
		env.push<bool>(false);
		expected = branchA;
	}

	op(env);
	REQUIRE(env.get_stack<bool>().empty());
	REQUIRE(env.get_stack<Code>().empty());
	REQUIRE(env.get_stack<Exec>().size() == 1);
	REQUIRE(env.pop<Exec>() == Code(expected));
}

TEST_CASE("Instruction: code_insert") {
	Env env;
	Instruction op(code_insert, "code_insert");
	auto vec = generate_test_values<Code>(4);

	/*
	 * 0: ( B C ( A ) )
	 * 1: B
	 * 2: C
	 * 3: ( A )
	 * 4: A
	*/

	CodeList container({
		vec[1],
		vec[2],
		CodeList({ vec[0] })
	});
	std::vector<Code> expected({
		vec[3],
		CodeList({ vec[3], vec[2], CodeList({ vec[0] }) }),
		CodeList({ vec[1], vec[3], CodeList({ vec[0] }) }),
		CodeList({ vec[1], vec[2], vec[3] }),
		CodeList({ vec[1], vec[2], CodeList({ vec[3] }) })
	});

	std::function<int(int)> get_index;
	SECTION("Positive indexes") {
		get_index = [&](int i) { return i; };
	}
	SECTION("Positive indexes out of bounds") {
		get_index = [&](int i) { return i + container.size(); };
	}
	SECTION("Negative indexes") {
		get_index = [&](int i) { return -i; };
	}
	SECTION("Negative indexes out of bounds") {
		get_index = [&](int i) { return -i - container.size(); };
	}
	for (unsigned i = 0; i < container.size(); ++i){
		env.push<Code>(vec[3]);
		env.push<Code>(container);
		env.push<int>(get_index(i));

		op(env);
		REQUIRE(env.get_stack<int>().empty());
		REQUIRE(env.get_stack<Code>().size() == 1);
		REQUIRE(env.pop<Code>() == expected[i]);
	}
}

TEST_CASE("Instruction: code_length") {
	Env env;
	Instruction op(code_length, "code_length");
	auto vec = generate_test_values<Code>(3);

	/*
	 * ( B C ( A ) ) -> 3
	 * ( A ) -> 1
	 * () -> 0
	 * A -> 1
	 */

	std::vector<Code> cases{
		CodeList({ vec[1], vec[2], CodeList({ vec[0] }) }),
		CodeList({ vec[0] }),
		CodeList(),
		vec[0]
	};

	for (auto el : cases) {
		env.push<Code>(el);
		op(env);
		REQUIRE(env.get_stack<Code>().empty());
		REQUIRE(env.get_stack<int>().size() == 1);
		if (is_list(el)) {
			REQUIRE(env.pop<int>() == static_cast<int>(std::get<CodeList>(el).get_list().size()));
		} else {
			REQUIRE(env.pop<int>() == 1);
		}
	}
}

TEST_CASE("Instruction: code_list") {
	Env env;
	Instruction op(code_list, "code_list");
	auto vec = generate_test_values<Code>(2);

	/*
	 * ( B ) A -> ( A ( B ) )
	*/

	env.push<Code>(vec[0]);
	env.push<Code>(CodeList({ vec[1] }));
	op(env);
	REQUIRE(env.get_stack<Code>() == std::vector<Code>{
		CodeList({ vec[0], CodeList({ vec[1] }) })
	});
}

TEST_CASE("Instruction: code_member") {
	Env env;
	Instruction op(code_member, "code_member");
	auto vec = generate_test_values<Code>(3);
	bool expected;

	env.push<Code>(vec[2]);
	SECTION("( A B ( C ) ) C") {
		env.push<Code>(CodeList({ vec[0], vec[1], CodeList({ vec[2] }) }));
		expected = false;
	}

	SECTION("( A C ( B ) ) C") {
		env.push<Code>(CodeList({ vec[0], vec[2], CodeList({ vec[1] }) }));
		expected = true;
	}

	op(env);
	REQUIRE(env.get_stack<Code>().empty());
	REQUIRE(env.get_stack<bool>() == std::vector<bool>{expected});
}

TEST_CASE("Instruction: code_noop") {
	Env env;
	Instruction op(code_noop, "code_noop");

	env.push<Exec>(op);
	auto exec_stack_copy = env.get_stack<Exec>();
	op(env);
	REQUIRE(env.get_stack<Exec>() == exec_stack_copy);
}

TEST_CASE("Instruction: code_nth") {
	Env env;
	Instruction op(code_nth, "code_nth");
	auto vec = generate_test_values<Code>(3);

	/*
	 * ( B C ( A ) )
	*/

	auto container = CodeList({ vec[1], vec[2], CodeList({ vec[0] }) });
	Code expected = CodeList();
	std::vector<Code> expected_list{
		vec[1],
		vec[2],
		CodeList({ vec[0] })
	};

	SECTION("Empty list") {
		env.push<Code>(CodeList());
		env.push<int>(0);
		expected = CodeList();
	}

	SECTION("Atom") {
		env.push<Code>(vec[0]);
		env.push<int>(0);
		expected = vec[0];
	}

	SECTION("Nth") {
		env.push<Code>(container);
		env.push<int>(2);
		expected = CodeList({ vec[0] });
	}

	SECTION("Nth out of bounds") {
		env.push<Code>(container);
		env.push<int>(5);
		expected = CodeList({ vec[0] });
	}

	SECTION("Nth negative") {
		env.push<Code>(container);
		env.push<int>(-2);
		expected = CodeList({ vec[0] });
	}

	SECTION("Nth negative out of bounds") {
		env.push<Code>(container);
		env.push<int>(-5);
		expected = CodeList({ vec[0] });
	}

	op(env);
	REQUIRE(env.get_stack<int>().empty());
	REQUIRE(env.get_stack<Code>() == std::vector<Code>{expected});
}

TEST_CASE("Instruction: code_nthcdr") {
	Env env;
	Instruction op(code_nthcdr, "code_nthcdr");
	auto vec = generate_test_values<Code>(3);

	/*
	 * ( B C ( A ) )
	 */
	CodeList container({ vec[1], vec[2], CodeList({ vec[0] }) });
	Code expected = CodeList();

	SECTION("Empty list: () -> ()") {
		env.push<Code>(CodeList());
		env.push<int>(1);
		expected = CodeList();
	}

	// Atoms coerced to list if necessary
	SECTION("Atom") {
		env.push<Code>(vec[0]);
		SECTION("0 A -> (A)") {
			env.push<int>(0);
			expected = CodeList({ vec[0] });
		}
		SECTION("1 A -> (A)") {
			env.push<int>(1);
			expected = CodeList({ vec[0] });
		}
	}

	SECTION("N argument") {
		env.push<Code>(container);

		expected = CodeList({ vec[2], CodeList({ vec[0] }) });
		SECTION("Nth") {
			SECTION("in-bounds") { env.push<int>(1); }
			SECTION("in-bounds 2") {
				env.push<int>(2);
				expected = CodeList({ Code(CodeList({ vec[0] })) });
			}
			SECTION("out of bounds") { env.push<int>(4); }
		}
		SECTION("Nth negative") {
			SECTION("in-bounds") { env.push<int>(-1); }
			SECTION("in-bounds -2") {
				env.push<int>(-2);
				expected = CodeList({ Code(CodeList({ vec[0] })) });
			}
			SECTION("out of bounds") { env.push<int>(-4); }
		}
	}

	op(env);
	REQUIRE(env.get_stack<int>().empty());
	REQUIRE(env.get_stack<Code>() == std::vector<Code>{expected});
}

TEST_CASE("Instruction: code_null") {
	Env env;
	Instruction op(code_null, "code_null");
	bool expected;

	SECTION("Empty list") {
		env.push<Code>(CodeList());
		expected = true;
	}
	SECTION("Not an empty list") {
		env.push<Code>(Literal(0));
		expected = false;
	}

	op(env);
	REQUIRE(env.get_stack<Code>().empty());
	REQUIRE(env.get_stack<bool>() == std::vector<bool>{expected});
}

TEST_CASE("Instruction: code_position") {
	Env env;
	Instruction op(code_position, "code_position");
	auto vec = generate_test_values<Code>(3);
	int expected;

	SECTION("Coerce first item to list") {
		env.push<Code>(vec[0]);
		env.push<Code>(vec[0]);
		expected = 0;
	}
	SECTION("-1 when not found") {
		env.push<Code>(vec[2]);
		env.push<Code>(CodeList({ vec[0], vec[1] }));
		expected = -1;
	}
	SECTION("Position found") {
		env.push<Code>(vec[1]);
		env.push<Code>(CodeList({ vec[0], vec[0], vec[1] }));
		expected = 2;
	}

	op(env);
	REQUIRE(env.get_stack<Code>().empty());
	REQUIRE(env.get_stack<int>() == std::vector<int>{expected});
}

TEST_CASE("Instruction: code_size") {
	Env env;
	Instruction op(code_size, "code_size");
	auto vec = generate_test_values<Code>(3);
	int expected;

	/*
	 * () = 1
	 * A = 1
	 * ( A A ) = 3
	 * ( A ( A A ) ) = 5
	 */
	SECTION("() = 1") {
		env.push<Code>(CodeList());
		expected = 1;
	}
	SECTION("A = 1") {
		env.push<Code>(vec[0]);
		expected = 1;
	}
	SECTION("( A A ) = 3") {
		env.push<Code>(CodeList({ vec[0], vec[1] }));
		expected = 3;
	}
	SECTION("( A ( A A ) ) = 5") {
		env.push<Code>(CodeList({ vec[0], CodeList({ vec[1], vec[2] }) }));
		expected = 5;
	}

	op(env);
	REQUIRE(env.get_stack<Code>().empty());
	REQUIRE(env.get_stack<int>() == std::vector<int>{expected});
}

TEST_CASE("Instruction: code_subst") {
	Env env;
	Instruction op(code_subst, "code_subst");
	auto vec = generate_test_values<Code>(3);
	Code expected = CodeList();

	/*
	 * A B B = A
	 * A B A = B
	 * A B C = A
	 * ( A B ) C B = ( A C )
	 */
	SECTION("A B B = A") {
		env.push<Code>(vec[1]);
		env.push<Code>(vec[1]);
		env.push<Code>(vec[0]);
		expected = vec[0];
	}
	SECTION("A B A = B") {
		env.push<Code>(vec[0]);
		env.push<Code>(vec[1]);
		env.push<Code>(vec[0]);
		expected = vec[1];
	}
	SECTION("A B C = A") {
		env.push<Code>(vec[2]);
		env.push<Code>(vec[1]);
		env.push<Code>(vec[0]);
		expected = vec[0];
	}
	SECTION("( A B ) C B = ( A C )") {
		env.push<Code>(vec[1]);
		env.push<Code>(vec[2]);
		env.push<Code>(CodeList({ vec[0], vec[1] }));
		expected = CodeList({ vec[0], vec[2] });
	}

	op(env);
	REQUIRE(env.get_stack<Code>() == std::vector<Code>{expected});
}
