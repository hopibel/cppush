#ifndef COMMON_OPS_H
#define COMMON_OPS_H

#include "code.h"
#include "env.h"

#include <algorithm>
#include <type_traits>

namespace cppush {

template <typename T>
unsigned equal(Env& env) {
	if (env.get_stack<T>().size() >= 2) {
		auto first = env.pop<T>();
		auto second = env.pop<T>();
		env.push<bool>(first == second);
	}
	return 1;
}
template <> unsigned equal<Code>(Env& env);
template <> unsigned equal<Exec>(Env& env);

template <typename T>
unsigned protected_pop(Env& env) {
	if (!env.get_stack<T>().empty()) {
		env.pop<T>();
	}
	return 1;
}

template <typename T>
unsigned dup(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() > 0) {
		stack.push_back(stack.back());
	}
	return 1;
}

template <typename T>
unsigned flush(Env& env) {
	env.get_stack<T>().clear();
	return 1;
}

template <typename T>
unsigned rot(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 3) {
		int index = stack.size() - 3;
		std::rotate(stack.begin() + index, stack.begin() + index + 1, stack.end());
	}
	return 1;
}

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

template <typename T>
unsigned stackdepth(Env& env) {
	env.get_stack<int>().push_back(env.get_stack<T>().size());
	return 1;
}

template <typename T>
unsigned swap(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		int index = stack.size() - 2;
		std::rotate(stack.begin() + index, stack.begin() + index + 1, stack.end());
	}
	return 1;
}

template <typename T>
unsigned yank(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() > 0 && env.get_stack<int>().size() > 0) {
		// yank<int> requires 2 items on the stack
		if (std::is_same<T, int>::value && env.get_stack<int>().size() < 2) {
			return 1;
		}

		int index = env.pop<int>();
		index = index < 0 ? 0 : (index >= static_cast<int>(stack.size()) ? stack.size()-1 : index);
		index = stack.size()-1 - index;

		std::rotate(stack.begin() + index, stack.begin() + index + 1, stack.end());

		return stack.size() - index;
	}
	return 1;
}

template <typename T>
unsigned yankdup(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() > 0 && env.get_stack<int>().size() > 0) {
		// yankdup<int> requires 2 items on the stack
		if (std::is_same<T, int>::value && env.get_stack<int>().size() < 2) {
			return 1;
		}

		int index = env.pop<int>();
		index = index < 0 ? 0 : (index >= static_cast<int>(stack.size()) ? stack.size()-1 : index);
		index = stack.size()-1 - index;

		stack.push_back(stack[index]);
	}
	return 1;
}

} // namespace cppush

#endif // COMMON_OPS_H
