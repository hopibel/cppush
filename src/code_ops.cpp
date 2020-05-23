#include "code.h"
#include "code_ops.h"
#include "common_ops.h"
#include "env.h"
#include "util.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <variant>

namespace cppush {

unsigned code_append(Env& env) {
	if (env.get_stack<Code>().size() >= 2) {
		auto first = env.pop<Code>();
		auto second = env.pop<Code>();

		std::vector<Code> first_list;
		if (is_atom(first)) {
			first_list.push_back(first);
		} else {
			first_list = std::get<CodeList>(first).get_list();
		}

		std::vector<Code> second_list;
		if (is_atom(second)) {
			second_list.push_back(second);
		} else {
			second_list = std::get<CodeList>(second).get_list();
		}

		second_list.insert(second_list.end(), first_list.begin(), first_list.end());
		env.push<Code>(CodeList(second_list));

		return first_list.size();
	}
	return 1;
}

unsigned code_atom(Env& env) {
	if (env.get_stack<Code>().size() > 0) {
		env.push<bool>(is_atom(env.pop<Code>()));
	}
	return 1;
}

unsigned code_car(Env& env) {
	auto& stack = env.get_stack<Code>();
	if (stack.size() > 0) {
		auto& first = stack.back();
		if (is_list(first)) {
			auto first_list = std::get<CodeList>(first).get_list();
			if (first_list.size() > 0) {
				stack.back() = first_list.front();
			}
		}
	}
	return 1;
}

unsigned code_cdr(Env& env) {
	auto& stack = env.get_stack<Code>();
	if (stack.size() > 0) {
		auto& first = stack.back();
		if (size(first) > 1) {
			auto first_list = std::get<CodeList>(first).get_list();
			first_list.erase(first_list.begin());
			stack.back() = CodeList(first_list);
		} else {
			stack.back() = CodeList();
		}
	}
	return 1;
}

// prepend second item on stack to first. if second is a list, don't expand
unsigned code_cons(Env& env) {
	if (env.get_stack<Code>().size() >= 2) {
		auto first = env.pop<Code>();
		auto second = env.pop<Code>();

		std::vector<Code> vec{second};
		if (is_list(first)) {
			auto& first_list = std::get<CodeList>(first).get_list();
			vec.insert(vec.end(), first_list.begin(), first_list.end());
		} else {
			vec.push_back(first);
		}
		env.push<Code>(CodeList(vec));
	}
	return 1;
}

namespace detail {

const Code find_container(const Code& tree, const Code& subtree) {
	// check if we're already in the container
	if (is_list(tree)) {
		auto& code_list = std::get<CodeList>(tree).get_list();
		for (const auto& node : code_list) {
			if (node == subtree) {
				return tree;
			}
		}

		// recurse
		for (const auto& node : code_list) {
			auto& container = find_container(node, subtree);
			if (std::get<CodeList>(container).get_list().size() > 0) {
				return container; // nonempty list means match
			}
		}
	}

	// no match. return empty list
	return CodeList();
}

bool contains(const Code tree, const Code subtree) {
	if (is_list(tree)) {
		auto& code_list = std::get<CodeList>(tree).get_list();
		for (const auto& el : code_list) {
			if (contains(el, subtree)) {
				return true;
			}
		}
	}
	return tree == subtree; // equality also counts as subset I guess
}

} // namespace detail

unsigned code_container(Env& env) {
	if (env.get_stack<Code>().size() >= 2) {
		auto first = env.pop<Code>();
		auto second = env.pop<Code>();

		env.push<Code>(detail::find_container(first, second));

		return size(first) * size(second);
	}
	return 1;
}

unsigned code_contains(Env& env) {
	if (env.get_stack<Code>().size() >= 2) {
		auto first = env.pop<Code>();
		auto second = env.pop<Code>();

		env.push<bool>(detail::contains(second, first));

		return size(first) * size(second);
	}
	return 1;
}

unsigned code_do(Env& env) {
	auto& stack = env.get_stack<Code>();
	if (stack.size() > 0) {
		static const auto code_pop = Instruction(protected_pop<Code>, "code_pop");
		env.push<Exec>(code_pop);
		env.push<Exec>(stack.back());
	}
	return 1;
}

unsigned code_do_star(Env& env) {
	if (env.get_stack<Code>().size() > 0) {
		env.push<Exec>(env.pop<Code>());
	}
	return 1;
}

unsigned code_quote(Env& env) {
	if (!env.get_stack<Exec>().empty()) {
		env.push<Code>(env.pop<Exec>());
	}
	return 1;
}

unsigned code_do_range(Env& env) {
	auto code_stack_size = env.get_stack<Code>().size();
	auto int_stack_size = env.get_stack<int>().size();

	if (code_stack_size > 0 && int_stack_size >= 2) {
		int dest = env.pop<int>();
		int index = env.get_stack<int>().back();
		auto code = env.pop<Code>();

		if (index != dest) {
			index += index > dest ? -1 : 1;

			// TODO: make these global to the file?
			static const auto do_range_insn = Instruction(code_do_range, "code_do*range");
			static const auto quote_insn = Instruction(code_quote, "code_quote");

			env.push<Exec>(CodeList({
				Literal(index), Literal(dest), quote_insn, code, do_range_insn
			}));
		}
		env.push<Exec>(code);
	}
	return 1;
}

unsigned code_do_count(Env& env) {
	auto code_stack_size = env.get_stack<Code>().size();
	auto int_stack_size = env.get_stack<int>().size();

	if (code_stack_size > 0 && int_stack_size > 0 && env.get_stack<int>().back() > 0) {
		int count = env.pop<int>();
		auto code = env.pop<Code>();

		static const auto do_range_insn = Instruction(code_do_range, "code_do*range");
		static const auto quote_insn = Instruction(code_quote, "code_quote");

		env.push<Exec>(CodeList({
			Literal(0), Literal(count - 1), quote_insn, code, do_range_insn
		}));
	}
	return 1;
}

unsigned code_do_times(Env& env) {
	auto& code_stack = env.get_stack<Code>();
	auto& int_stack = env.get_stack<int>();

	if (code_stack.size() > 0 && int_stack.size() > 0 && int_stack.back() > 0) {
		int times = env.pop<int>();
		auto code = env.pop<Code>();

		static const auto do_range_insn = Instruction(code_do_range, "code_do*range");
		static const auto quote_insn = Instruction(code_quote, "code_quote");
		static const auto int_pop = Instruction(protected_pop<int>, "integer_pop");

		env.push<Exec>(CodeList({
			Literal(0), Literal(times - 1),
			quote_insn, CodeList({ int_pop, code }), do_range_insn
		}));
	}
	return 1;
}

namespace detail {

// assumptions: 0 <= index < code->size()
const Code extract_recursive(const Code& code, unsigned index) {
	if (index == 0) {
		return code;
	}
	index -= 1;
	const auto& code_list = std::get<CodeList>(code).get_list();
	for (auto el : code_list) {
		if (index < size(el)) {
			return extract_recursive(el, index);
		}
		index -= size(el);
	}

	// shouldn't reach
	return code;
}

}

unsigned code_extract(Env& env) {
	if (env.get_stack<Code>().size() > 0 && env.get_stack<int>().size() > 0) {
		auto code = env.get_stack<Code>().back();
		auto index = env.pop<int>();

		index = std::abs(index) % size(code); // restrict to reasonable range
		if (index == 0) { // no need to change code stack
			return 1;
		}
		env.pop<Code>();

		env.push<Code>(detail::extract_recursive(code, index));
		return size(code);
	}
	return 1;
}

unsigned code_from_bool(Env& env) {
	if (env.get_stack<bool>().size() > 0) {
		env.push<Code>(Literal(env.pop<bool>()));
	}
	return 1;
}

unsigned code_from_float(Env& env) {
	if (env.get_stack<double>().size() > 0) {
		env.push<Code>(Literal(env.pop<double>()));
	}
	return 1;
}

unsigned code_from_int(Env& env) {
	if (env.get_stack<int>().size() > 0) {
		env.push<Code>(Literal(env.pop<int>()));
	}
	return 1;
}

unsigned code_if(Env& env) {
	if (env.get_stack<Code>().size() >= 2 && env.get_stack<bool>().size() > 0) {
		if (env.pop<bool>()) {
			env.pop<Code>();
			env.push<Exec>(env.pop<Code>());
		} else {
			env.push<Exec>(env.pop<Code>());
			env.pop<Code>();
		}
	}
	return 1;
}

namespace detail {

// insert subtree into code at index (based on depth-first traversal)
const Code insert_recursive(Env& env, const Code& code, const Code& subtree, unsigned index) {
	if (index == 0) {
		return subtree;
	}
	index -= 1;

	auto code_list = std::get<CodeList>(code).get_list();
	for (unsigned i = 0; i < code_list.size(); ++i) {
		if (index < size(code_list[i])) {
			auto new_code = insert_recursive(env, code_list[i], subtree, index);
			code_list[i] = new_code;
			return CodeList(code_list);
		}
		index -= size(code_list[i]);
	}

	// shouldn't reach
	return subtree;
}

}

unsigned code_insert(Env& env) {
	if (env.get_stack<Code>().size() >= 2 && env.get_stack<int>().size() > 0) {
		auto first = env.pop<Code>();
		auto second = env.pop<Code>();
		int index = env.pop<int>();

		index = std::abs(index) % size(first);
		auto result = detail::insert_recursive(env, first, second, index);

		env.push<Code>(result);
	}
	return 1;
}

unsigned code_length(Env& env) {
	if (env.get_stack<Code>().size() > 0) {
		auto code = env.pop<Code>();
		if (is_list(code)) {
			env.push<int>(std::get<CodeList>(code).get_list().size());
		} else {
			env.push<int>(1);
		}
	}
	return 1;
}

unsigned code_list(Env& env) {
	if (env.get_stack<Code>().size() >= 2) {
		auto first = env.pop<Code>();
		auto second = env.pop<Code>();
		env.push<Code>(CodeList({ second, first }));
	}
	return 1;
}

unsigned code_member(Env& env) {
	if (env.get_stack<Code>().size() >= 2) {
		auto first = env.pop<Code>();
		auto second = env.pop<Code>();

		if (is_atom(first)) {
			env.push<bool>(first == second);
		} else {
			for (const auto& el : std::get<CodeList>(first).get_list()) {
				if (el == second) {
					env.push<bool>(true);
					return 1;
				}
			}
			env.push<bool>(false);
		}
	}
	return 1;
}

unsigned code_noop(Env&) { return 1; }

unsigned code_nth(Env& env) {
	auto& stack = env.get_stack<Code>();
	if (stack.size() > 0 && env.get_stack<int>().size() > 0) {
		int index = env.pop<int>();
		if (is_list(stack.back())) {
			const auto& code_list = std::get<CodeList>(stack.back()).get_list();
			if (code_list.size() > 0) {
				index = std::abs(index) % code_list.size();
				stack.back() = code_list[index];
			}
		}
	}
	return 1;
}

unsigned code_nthcdr(Env& env) {
	if (env.get_stack<Code>().size() > 0 && env.get_stack<int>().size() > 0) {
		int index = env.pop<int>();
		const auto code = env.pop<Code>();
		std::vector<Code> code_list;

		// coerce to list if necessary
		if (is_list(code)) {
			code_list = std::get<CodeList>(code).get_list();
		} else {
			code_list = {code};
		}

		if (code_list.size()) {
			index = std::abs(index) % code_list.size();
			code_list.erase(code_list.begin(), code_list.begin() + index);
		}
		env.push<Code>(CodeList(code_list));

		return code_list.size() - index;
	}
	return 1;
}

unsigned code_null(Env& env) {
	if (env.get_stack<Code>().size() > 0) {
		std::visit(overloaded{
			[&](CodeList arg) { env.push<bool>(arg.get_list().empty()); },
			[&](auto&&) { env.push<bool>(false); }
		}, env.pop<Code>());
	}
	return 1;
}

unsigned code_position(Env& env) {
	if (env.get_stack<Code>().size() >= 2) {
		auto first = env.pop<Code>();
		auto second = env.pop<Code>();

		if (is_atom(first)) {
			env.push<int>(first == second ? 0 : -1);
			return 1;
		}

		const auto& code_list = std::get<CodeList>(first).get_list();
		for (unsigned i = 0; i < code_list.size(); ++i) {
			if (code_list[i] == second) {
				env.push<int>(i);
				return code_list.size() * size(second);
			}
		}

		// not found
		env.push<int>(-1);
		return code_list.size() * size(second);
	}
	return 1;
}

unsigned code_size(Env& env) {
	if (env.get_stack<Code>().size() > 0) {
		auto code = env.pop<Code>();
		env.push<int>(size(code));
	}
	return 1;
}

unsigned code_subst(Env& env) {
	if (env.get_stack<Code>().size() >= 3) {
		auto first = env.pop<Code>();
		auto second = env.pop<Code>();
		auto third = env.pop<Code>();

		int fsize = size(first);
		int ssize = size(second);
		int tsize = size(third);

		// second == third?
		if (second == third) {
			env.push<Code>(first);
			return std::min(ssize, tsize);
		} else if (is_atom(first)) {
			if (first == third) {
				env.push<Code>(second);
			} else {
				env.push<Code>(first);
			}
			return std::min(fsize, tsize);
		} else {
			auto code_list = std::get<CodeList>(first).get_list();
			for (unsigned i = 0; i < code_list.size(); ++i) {
				if (code_list[i] == third) {
					code_list[i] = second;
				}
			}
			env.push<Code>(CodeList(code_list));
			return std::min(fsize, tsize);
		}
	}
	return 1;
}

} // namespace cppush
