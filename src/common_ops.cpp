#include "code.h"
#include "common_ops.h"
#include "env.h"

namespace cppush {

namespace detail {
unsigned code_equal_impl(Env& env, std::vector<Code_ptr>& stack) {
	if (stack.size() >= 2) {
		auto first = stack.back();
		stack.pop_back();
		auto second = stack.back();
		stack.pop_back();

		env.get_stack<bool>().push_back(*first == *second);

		return first->size() < second->size() ? first->size() : second->size();
	}
	return 1;
}
} // namespace detail

template <>
unsigned equal<Code_ptr>(Env& env) {return detail::code_equal_impl(env, env.get_stack<Code_ptr>());}
unsigned exec_equal(Env& env) {return detail::code_equal_impl(env, env.get_exec_stack());}

// can't template because exec_stack is same type as code_stack
unsigned protected_exec_pop(Env& env) {
	if (!env.get_exec_stack().empty()) {
		env.pop_exec();
	}
	return 1;
}

unsigned exec_dup(Env& env) {
	auto& stack = env.get_exec_stack();
	if (stack.size() > 0) {
		stack.push_back(stack.back());
	}
	return 1;
}

unsigned exec_flush(Env& env) {
	env.get_exec_stack().clear();
	return 1;
}

unsigned exec_rot(Env& env) {
	auto& stack = env.get_exec_stack();
	if (stack.size() >= 3) {
		auto first = env.pop_exec();
		auto second = env.pop_exec();
		auto third = env.pop_exec();

		stack.push_back(second);
		stack.push_back(first);
		stack.push_back(third);
	}
	return 1;
}

unsigned exec_shove(Env& env) {
	auto& stack = env.get_exec_stack();
	if (stack.size() > 0 && env.get_stack<int>().size() > 0) {
		int index = env.pop<int>();
		index = index < 0 ? 0 : (static_cast<size_t>(index) >= stack.size() ? stack.size()-1 : index);
		index = stack.size()-1 - index;
		stack.insert(stack.begin() + index, env.pop_exec());

		return stack.size() - index + 1;
	}
	return 1;
}

unsigned exec_stackdepth(Env& env) {
	env.get_stack<int>().push_back(env.get_exec_stack().size());
	return 1;
}

unsigned exec_swap(Env& env) {
	auto& stack = env.get_exec_stack();
	if (stack.size() >= 2) {
		auto first = env.pop_exec();
		auto second = env.pop_exec();
		stack.push_back(first);
		stack.push_back(second);
	}
	return 1;
}

unsigned exec_yank(Env& env) {
	auto& stack = env.get_exec_stack();
	if (stack.size() > 0 && env.get_stack<int>().size() > 0) {
		int index = env.pop<int>();
		index = index < 0 ? 0 : (index >= static_cast<int>(stack.size()) <= index ? stack.size()-1 : index);
		index = stack.size()-1 - index;

		auto value = stack[index];
		stack.erase(stack.begin() + index);
		stack.push_back(value);

		return stack.size() - index + 1;
	}
	return 1;
}

unsigned exec_yankdup(Env& env) {
	auto& stack = env.get_exec_stack();
	if (stack.size() > 0 && env.get_stack<int>().size() > 0) {
		int index = env.pop<int>();
		index = index < 0 ? 0 : (index >= static_cast<int>(stack.size()) ? stack.size()-1 : index);
		index = stack.size()-1 - index;

		stack.push_back(stack[index]);
	}
	return 1;
}

} // namespace cppush

