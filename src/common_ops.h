#ifndef COMMON_OPS_H
#define COMMON_OPS_H

#include "code.h"
#include "env.h"

namespace cppush {

template <typename T>
unsigned equal(Env& env) {
	if (env.get_stack<T>().size() >= 2) {
		auto first = env.pop<T>();
		auto second = env.pop<T>();
		env.get_stack<bool>().push_back(first == second);
	}
	return 1;
}
template <>
unsigned equal<Code_ptr>(Env& env);
unsigned exec_equal(Env& env);

template <typename T>
unsigned protected_pop(Env& env) {
	if (!env.get_stack<T>().empty()) {
		env.pop<T>();
	}
	return 1;
}
// can't template because exec_stack is same type as code_stack
unsigned protected_exec_pop(Env& env);

template <typename T>
unsigned dup(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() > 0) {
		stack.push_back(stack.back());
	}
	return 1;
}
unsigned exec_dup(Env& env);

template <typename T>
unsigned flush(Env& env) {
	env.get_stack<T>().clear();
	return 1;
}
unsigned exec_flush(Env& env);

template <typename T>
unsigned rot(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 3) {
		auto first = env.pop<T>();
		auto second = env.pop<T>();
		auto third = env.pop<T>();

		stack.push_back(second);
		stack.push_back(first);
		stack.push_back(third);
	}
	return 1;
}
unsigned exec_rot(Env& env);

template <typename T>
unsigned shove(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() > 0 && env.get_stack<int>().size() > 0) {
		int index = env.pop<int>();
		index = index < 0 ? 0 : (static_cast<size_t>(index) >= stack.size() ? stack.size()-1 : index);
		index = stack.size()-1 - index;
		stack.insert(stack.begin() + index, env.pop<T>());

		return stack.size() - index + 1;
	}
	return 1;
}
unsigned exec_shove(Env& env);

template <typename T>
unsigned stackdepth(Env& env) {
	env.get_stack<int>().push_back(env.get_stack<T>().size());
	return 1;
}
unsigned exec_stackdepth(Env& env);

template <typename T>
unsigned swap(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		auto first = env.pop<T>();
		auto second = env.pop<T>();
		stack.push_back(first);
		stack.push_back(second);
	}
	return 1;
}
unsigned exec_swap(Env& env);

template <typename T>
unsigned yank(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() > 0 && env.get_stack<int>().size() > 0) {
		int index = env.pop<int>();
		index = index < 0 ? 0 : (index >= static_cast<int>(stack.size()) ? stack.size()-1 : index);
		index = stack.size()-1 - index;

		auto value = stack[index];
		stack.erase(stack.begin() + index);
		stack.push_back(value);

		return stack.size() - index + 1;
	}
	return 1;
}
unsigned exec_yank(Env& env);

template <typename T>
unsigned yankdup(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 0 && env.get_stack<int>().size() > 0) {
		int index = env.pop<int>();
		index = index < 0 ? 0 : (index >= static_cast<int>(stack.size()) ? stack.size()-1 : index);
		index = stack.size()-1 - index;

		stack.push_back(stack[index]);
	}
	return 1;
}
unsigned exec_yankdup(Env& env);

} // namespace cppush

#endif // COMMON_OPS_H
