#ifndef CODE_OPS_H
#define CODE_OPS_H

#include "code.h"
#include "common_ops.h"
#include "env.h"
#include "instruction.h"
#include "literal.h"

#include <algorithm>
#include <cmath>
#include <memory>

namespace cppush {

inline unsigned code_append(Env& env) {
	auto& stack = env.get_stack<Code_ptr>();
	if (stack.size() >= 2) {
		auto first = env.pop<Code_ptr>();
		auto second = env.pop<Code_ptr>();

		// important! copy, not reference
		auto first_stack = first->get_stack();
		if (first->is_atom()) {
			first_stack.push_back(first);
		}

		auto second_stack = second->get_stack();
		if (second->is_atom()) {
			second_stack.push_back(second);
		}

		second_stack.insert(second_stack.end(), first_stack.begin(), first_stack.end());
		stack.push_back(std::make_shared<CodeList>(second_stack));

		return first_stack.size();
	}
	return 1;
}

inline unsigned code_atom(Env& env) {
	auto& stack = env.get_stack<Code_ptr>();
	if (stack.size() > 0) {
		auto first = env.pop<Code_ptr>();
		env.get_stack<bool>().push_back(first->is_atom());
	}
	return 1;
}

inline unsigned code_car(Env& env) {
	auto& stack = env.get_stack<Code_ptr>();
	if (stack.size() > 0) {
		auto first = stack.back();

		if (!first->get_stack().empty()) {
			stack.back() = first->get_stack().front();
		}
	}
	return 1;
}

inline unsigned code_cdr(Env& env) {
	auto& stack = env.get_stack<Code_ptr>();
	if (stack.size() > 0) {
		auto top_stack = stack.back()->get_stack();

		if (!top_stack.empty()) {
			top_stack.erase(top_stack.begin());
		}
		stack.back() = std::make_shared<CodeList>(top_stack);
	}
	return 1;
}

// prepend second item on stack to first. if second is a list, don't expand
inline unsigned code_cons(Env& env) {
	auto& stack = env.get_stack<Code_ptr>();
	if (stack.size() >= 2) {
		auto first = env.pop<Code_ptr>();
		auto second = env.pop<Code_ptr>();

		auto first_stack = first->get_stack();
		if (first->is_list()) {
			first_stack.insert(first_stack.begin(), second);
		} else {
			first_stack.push_back(second);
			first_stack.push_back(first);
		}

		stack.push_back(std::make_shared<CodeList>(first_stack));
	}
	return 1;
}

namespace detail {

inline std::shared_ptr<Code> find_container(std::shared_ptr<Code> tree, std::shared_ptr<Code> subtree) {
	// check if we're already in the container
	auto& stack = tree->get_stack();
	for (auto c : stack) {
		if (*c == *subtree) {
			return tree;
		}
	}

	// recurse
	for (auto c : stack) {
		auto container = find_container(c, subtree);
		if (!container->get_stack().empty()) {
			return container; // nonempty list means match
		}
	}

	// no match. return empty list
	return std::make_shared<CodeList>();
	}

inline bool contains(const std::shared_ptr<Code> tree, const std::shared_ptr<Code> subtree) {
	auto& stack = tree->get_stack();
	for (auto el : stack) {
		if (contains(el, subtree)) {
			return true;
		}
	}
	return *tree == *subtree; // equality also counts as subset I guess
}

} // namespace detail

inline unsigned code_container(Env& env) {
	auto& stack = env.get_stack<Code_ptr>();
	if (stack.size() >= 2) {
		auto first = env.pop<Code_ptr>();
		auto second = env.pop<Code_ptr>();

		stack.push_back(detail::find_container(first, second));

		return first->size() * second->size();
	}
	return 1;
}

inline unsigned code_contains(Env& env) {
	if (env.get_stack<Code_ptr>().size() >= 2) {
		auto first = env.pop<Code_ptr>();
		auto second = env.pop<Code_ptr>();

		env.get_stack<bool>().push_back(detail::contains(second, first));

		return first->size() * second->size();
	}
	return 1;
}

inline unsigned code_do(Env& env) {
	auto& stack = env.get_stack<Code_ptr>();
	if (stack.size() > 0) {
		static std::shared_ptr<Instruction> code_pop = std::make_shared<Instruction>(
				protected_code_pop,
				"CODE.POP",
				CODE);
		env.exec_stack.push_back(code_pop);
		env.exec_stack.push_back(stack.back());
	}
	return 1;
}

inline unsigned code_do_star(Env& env) {
	if (env.get_stack<Code_ptr>().size() > 0) {
		env.exec_stack.push_back(env.pop<Code_ptr>());
	}
	return 1;
}

inline unsigned code_quote(Env& env) {
	if (!env.exec_stack.empty()) {
		env.get_stack<Code_ptr>().push_back(env.exec_stack.back());
		env.exec_stack.pop_back();
	}
	return 1;
}

inline unsigned code_do_range(Env& env) {
	auto code_stack_size = env.get_stack<Code_ptr>().size();
	auto int_stack_size = env.get_stack<int>().size();

	if (code_stack_size > 0 && int_stack_size >= 2) {
		int dest = env.pop<int>();
		int index = env.get_stack<int>().back();
		auto code = env.pop<Code_ptr>();

		if (index != dest) {
			index += index > dest ? -1 : 1;

			static auto do_range_insn = std::make_shared<Instruction>(
					code_do_range,
					"CODE.DO*RANGE",
					CODE | EXEC | INT);
			static auto quote_insn = std::make_shared<Instruction>(
					code_quote,
					"CODE.QUOTE",
					CODE | EXEC,
					1);

			std::vector<std::shared_ptr<Code>> rcall{
					std::make_shared<Literal<int>>(index),
					std::make_shared<Literal<int>>(dest),
					quote_insn, code, do_range_insn
			};

			env.get_exec_stack().push_back(std::make_shared<CodeList>(rcall));
		}
		env.get_exec_stack().push_back(code);
	}
	return 1;
}

inline unsigned code_do_count(Env& env) {
	auto code_stack_size = env.get_stack<Code_ptr>().size();
	auto int_stack_size = env.get_stack<int>().size();

	if (code_stack_size > 0 && int_stack_size > 0 && env.get_stack<int>().back() > 0) {
		int count = env.pop<int>();
		auto code = env.pop<Code_ptr>();

		static auto do_range_insn = std::make_shared<Instruction>(
				code_do_range,
				"CODE.DO*RANGE",
				CODE | EXEC | INT);
		static auto quote_insn = std::make_shared<Instruction>(
				code_quote,
				"CODE.QUOTE",
				CODE | EXEC,
				1);

		std::vector<std::shared_ptr<Code>> rcall{
			std::make_shared<Literal<int>>(0),
			std::make_shared<Literal<int>>(count - 1),
			quote_insn, code, do_range_insn
		};

		env.get_exec_stack().push_back(std::make_shared<CodeList>(rcall));
	}
	return 1;
}

inline unsigned code_do_times(Env& env) {
	auto& code_stack = env.get_stack<Code_ptr>();
	auto& int_stack = env.get_stack<int>();

	if (code_stack.size() > 0 && int_stack.size() > 0 && int_stack.back() > 0) {
		int times = env.pop<int>();
		auto code = env.pop<Code_ptr>();

		static auto do_range_insn = std::make_shared<Instruction>(
				code_do_range,
				"CODE.DO*RANGE",
				CODE | EXEC | INT);
		static auto quote_insn = std::make_shared<Instruction>(
				code_quote,
				"CODE.QUOTE",
				CODE | EXEC,
				1);
		static auto int_pop = std::make_shared<Instruction>(
				protected_pop<int>,
				"INTEGER.POP",
				INT);

		std::vector<std::shared_ptr<Code>> pop_code{int_pop, code};

		std::vector<std::shared_ptr<Code>> rcall{
			std::make_shared<Literal<int>>(0),
			std::make_shared<Literal<int>>(times - 1),
			quote_insn, std::make_shared<CodeList>(pop_code), do_range_insn
		};

		env.get_exec_stack().push_back(std::make_shared<CodeList>(rcall));
	}
	return 1;
}

// TODO(hopibel): should probably factor these helper functions out into a private library
// remove inline when you do
namespace detail {

// assumptions: 0 <= index < code->size()
inline std::shared_ptr<Code> extract_recursive(std::shared_ptr<Code> code, int index) {
	if (index == 0) {
		return code;
	}
	index -= 1;
	const auto& stack = code->get_stack();
	for (auto el : stack) {
		if (index < static_cast<int>(el->size())) {
			return extract_recursive(el, index);
		}
		index -= el->size();
	}

	// shouldn't reach
	return code;
}

}

inline unsigned code_extract(Env& env) {
	auto& code_stack = env.get_stack<Code_ptr>();
	if (code_stack.size() > 0 && env.get_stack<int>().size() > 0) {
		auto code = code_stack.back();
		auto index = env.pop<int>();

		index = std::abs(index) % code->size(); // restrict to reasonable range
		if (index == 0) { // no need to change code stack
			return 1;
		}
		env.pop<Code_ptr>();

		code_stack.push_back(detail::extract_recursive(code, index));
		return code->size();
	}
	return 1;
}

inline unsigned code_from_bool(Env& env) {
	if (env.get_stack<bool>().size() > 0) {
		env.get_stack<Code_ptr>().push_back(
				std::make_shared<Literal<bool>>(env.pop<bool>()));
	}
	return 1;
}

inline unsigned code_from_float(Env& env) {
	if (env.get_stack<double>().size() > 0) {
		env.get_stack<Code_ptr>().push_back(
				std::make_shared<Literal<double>>(env.pop<double>()));
	}
	return 1;
}

inline unsigned code_from_int(Env& env) {
	if (env.get_stack<int>().size() > 0) {
		env.get_stack<Code_ptr>().push_back(
				std::make_shared<Literal<int>>(env.pop<int>()));
	}
	return 1;
}

inline unsigned code_if(Env& env) {
	if (env.get_stack<Code_ptr>().size() >= 2 && env.get_stack<bool>().size() > 0) {
		auto first = env.pop<Code_ptr>();
		auto second = env.pop<Code_ptr>();
		if (env.pop<bool>()) {
			env.get_exec_stack().push_back(second);
		} else {
			env.get_exec_stack().push_back(first);
		}
	}
	return 1;
}

namespace detail {

// insert subtree into code at index (based on depth-first traversal)
inline std::shared_ptr<Code> insert_recursive(std::shared_ptr<Code> code, std::shared_ptr<Code> subtree, int index) {
	if (index == 0) {
		return subtree;
	}
	index -= 1;

	auto stack = code->get_stack();
	for (int i = 0; i < static_cast<int>(stack.size()); ++i) {
		if (index < static_cast<int>(stack[i]->size())) {
			auto new_code = insert_recursive(stack[i], subtree, index);
			stack[i] = new_code;
			return std::make_shared<CodeList>(stack);
		}
		index -= stack[i]->size();
	}

	// shouldn't reach
	return subtree;
}

}

inline unsigned code_insert(Env& env) {
	auto& stack = env.get_stack<Code_ptr>();
	if (stack.size() >= 2 && env.get_stack<int>().size() > 0) {
		auto first = env.pop<Code_ptr>();
		auto second = env.pop<Code_ptr>();
		int index = env.pop<int>();

		index = std::abs(index) % first->size();
		auto result = detail::insert_recursive(first, second, index);

		stack.push_back(result);
	}
	return 1;
}

inline unsigned code_length(Env& env) {
	auto& stack = env.get_stack<Code_ptr>();
	if (stack.size() > 0) {
		auto code = env.pop<Code_ptr>();
		if (code->is_list()) {
			env.get_stack<int>().push_back(code->get_stack().size());
		} else {
			env.get_stack<int>().push_back(1);
		}
	}
	return 1;
}

inline unsigned code_list(Env& env) {
	auto& stack = env.get_stack<Code_ptr>();
	if (stack.size() >= 2) {
		auto first = env.pop<Code_ptr>();
		auto second = env.pop<Code_ptr>();

		std::vector<std::shared_ptr<Code>> list{second, first};
		stack.push_back(std::make_shared<CodeList>(list));
	}
	return 1;
}

inline unsigned code_member(Env& env) {
	if (env.get_stack<Code_ptr>().size() >= 2) {
		auto first = env.pop<Code_ptr>();
		auto second = env.pop<Code_ptr>();

		if (first->is_atom()) {
			env.get_stack<bool>().push_back(*first == *second);
		} else {
			for (auto el : first->get_stack()) {
				if (*el == *second) {
					env.get_stack<bool>().push_back(true);
					return 1;
				}
			}
			env.get_stack<bool>().push_back(false);
		}
	}
	return 1;
}

inline unsigned code_noop(Env&) {return 1;}

inline unsigned code_nth(Env& env) {
	auto& stack = env.get_stack<Code_ptr>();
	if (stack.size() > 0 && env.get_stack<int>().size() > 0) {
		int index = env.pop<int>();
		if (!stack.back()->get_stack().empty()) {
			auto code_list = env.pop<Code_ptr>()->get_stack();
			index = std::abs(index) % code_list.size();
			stack.push_back(code_list[index]);

			return code_list.size();
		}
	}
	return 1;
}

inline unsigned code_nthcdr(Env& env) {
	auto& stack = env.get_stack<Code_ptr>();
	if (stack.size() > 0 && env.get_stack<int>().size() > 0) {
		int index = env.pop<int>();
		if (!stack.back()->get_stack().empty()) {
			auto code_list = env.pop<Code_ptr>()->get_stack();
			index = std::abs(index) % code_list.size();
			code_list.erase(code_list.begin(), code_list.begin() + index);
			stack.push_back(std::make_shared<CodeList>(code_list));

			return code_list.size();
		}
	}
	return 1;
}

inline unsigned code_null(Env& env) {
	if (env.get_stack<Code_ptr>().size() > 0) {
		auto code = env.pop<Code_ptr>();
		if (code->is_list() && code->get_stack().empty()) {
			env.get_stack<bool>().push_back(true);
		} else {
			env.get_stack<bool>().push_back(false);
		}
	}
	return 1;
}

inline unsigned code_position(Env& env) {
	if (env.get_stack<Code_ptr>().size() >= 2) {
		auto first = env.pop<Code_ptr>();
		auto second = env.pop<Code_ptr>();
		const auto& stack = first->get_stack();

		if (stack.empty()) {
			env.get_stack<int>().push_back(*first == *second ? 0 : -1);
			return 1;
		}

		for (int i = 0; i < static_cast<int>(stack.size()); ++i) {
			if (*stack[i] == *second) {
				env.get_stack<int>().push_back(i);
				return stack.size() * second->size();
			}
		}

		env.get_stack<int>().push_back(-1);
		return stack.size() * second->size();
	}
	return 1;
}

inline unsigned code_size(Env& env) {
	if (env.get_stack<Code_ptr>().size() > 0) {
		auto code = env.pop<Code_ptr>();
		env.get_stack<int>().push_back(code->size());
	}
	return 1;
}

inline unsigned code_subst(Env& env) {
	auto& code_stack = env.get_stack<Code_ptr>();
	if (code_stack.size() >= 3) {
		auto first = env.pop<Code_ptr>();
		auto second = env.pop<Code_ptr>();
		auto third = env.pop<Code_ptr>();

		int fsize = first->size();
		int ssize = second->size();
		int tsize = third->size();

		// second == third?
		if (*second == *third) {
			code_stack.push_back(first);
			return ssize < tsize ? ssize : tsize;
		}

		// atom?
		if (first->is_atom()) {
			if (*first == *third) {
				code_stack.push_back(second);
			} else {
				code_stack.push_back(first);
			}
			return fsize < tsize ? fsize : tsize;
		}

		auto stack = first->get_stack();
		for (int i = 0; i < static_cast<int>(stack.size()); ++i) {
			if (*stack[i] == *third) {
				stack[i] = second;
			}
		}
		code_stack.push_back(std::make_shared<CodeList>(stack));
		return fsize < tsize ? fsize : tsize;
	}
	return 1;
}

} // namespace cppush

#endif // CODE_OPS_H
