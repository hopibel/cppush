#ifndef COMMON_OPS_H
#define COMMON_OPS_H

#include "env.h"

namespace cppush {

template <typename T>
inline unsigned equal(Env& env) {
	if (get_stack<T>(env).size() >= 2) {
		auto first = pop<T>(env);
		auto second = pop<T>(env);
		get_stack<bool>(env).push_back(first == second);

		int fsize = first->size();
		int ssize = second->size();
		return fsize < ssize ? fsize : ssize;
	}
	return 1;
}

template <>
inline unsigned equal<std::shared_ptr<Code>>(Env& env) {
	if (get_stack<std::shared_ptr<Code>>(env).size() >= 2) {
		auto first = pop<std::shared_ptr<Code>>(env);
		auto second = pop<std::shared_ptr<Code>>(env);
		get_stack<bool>(env).push_back(*first == *second);
		
		int fsize = first->size();
		int ssize = second->size();
		return fsize < ssize ? fsize : ssize;
	}
	return 1;
}

inline unsigned exec_equal(Env& env) {
	if (get_exec_stack(env).size() >= 2) {
		auto first = pop_exec(env);
		auto second = pop_exec(env);
		get_stack<bool>(env).push_back(*first == *second);

		int fsize = first->size();
		int ssize = second->size();
		return fsize < ssize ? fsize : ssize;
	}
	return 1;
}

template <typename T>
inline unsigned protected_pop(Env& env) {
	if (!get_stack<T>(env).empty()) {
		pop<T>(env);
	}
	return 1;
}

// can't template because exec_stack is same type as code_stack
inline unsigned protected_exec_pop(Env& env) {
	if (!get_exec_stack(env).empty()) {
		pop_exec(env);
	}
	return 1;
}

template <typename T>
inline unsigned dup(Env& env) {
	auto& stack = get_stack<T>(env);
	if (stack.size() > 0) {
		stack.push_back(stack.back());
	}
	return 1;
}

inline unsigned exec_dup(Env& env) {
	auto& stack = get_exec_stack(env);
	if (stack.size() > 0) {
		stack.push_back(stack.back());
	}
	return 1;
}

template <typename T>
inline unsigned flush(Env& env) {
	get_stack<T>(env).clear();
	return 1;
}

inline unsigned exec_flush(Env& env) {
	get_exec_stack(env).clear();
	return 1;
}

template <typename T>
inline unsigned rot(Env& env) {
	auto& stack = get_stack<T>(env);
	if (stack.size() >= 3) {
		auto first = pop<T>(env);
		auto second = pop<T>(env);
		auto third = pop<T>(env);

		stack.push_back(second);
		stack.push_back(first);
		stack.push_back(third);
	}
	return 1;
}

inline unsigned exec_rot(Env& env) {
	auto& stack = get_exec_stack(env);
	if (stack.size() >= 3) {
		auto first = pop<T>(env);
		auto second = pop<T>(env);
		auto third = pop<T>(env);

		stack.push_back(second);
		stack.push_back(first);
		stack.push_back(third);
	}
	return 1;
}

template <typename T>
inline unsigned shove(Env& env) {
	auto& stack = get_stack<T>(env);
	if (stack.size() > 0 && get_stack<int>(env).size() > 0) {
		int index = pop<int>(env);
		index = index < 0 ? 0 : (index >= stack.size() ? stack.size()-1 : index);
		index = stack.size()-1 - index;
		stack.insert(stack.begin() + index, pop<T>(env));

		return stack.size() - index + 1;
	}
	return 1;
}

inline unsigned exec_shove(Env& env) {
	auto& stack = get_exec_stack(env);
	if (stack.size() > 0 && get_stack<int>(env).size() > 0) {
		int index = pop<int>(env);
		index = index < 0 ? 0 : (index >= stack.size() ? stack.size()-1 : index);
		index = stack.size()-1 - index;
		stack.insert(stack.begin() + index, pop_exec(env));

		return stack.size() - index + 1;
	}
	return 1;
}

template <typename T>
inline unsigned stackdepth(Env& env) {
	get_stack<int>(env).push_back(get_stack<T>(env).size());
	return 1;
}

inline unsigned exec_stackdepth(Env& env) {
	get_stack<int>(env).push_back(get_exec_stack(env).size());
	return 1;
}

template <typename T>
inline unsigned swap(Env& env) {
	auto& stack = get_stack<T>(env);
	if (stack.size() >= 2) {
		auto first = pop<T>(env);
		auto second = pop<T>(env);
		stack.push_back(first);
		stack.push_back(second);
	}
	return 1;
}

inline unsigned exec_swap(Env& env) {
	auto& stack = get_exec_stack(env);
	if (stack.size() >= 2) {
		auto first = pop_exec(env);
		auto second = pop_exec(env);
		stack.push_back(first);
		stack.push_back(second);
	}
	return 1;
}

template <typename T>
inline unsigned yank(Env& env) {
	auto& stack = get_stack<T>(env);
	if (stack.size() >= 0 && get_stack<int>(env).size() > 0) {
		int index = pop<int>(env);
		index = index < 0 ? 0 : (index >= stack.size() ? stack.size()-1 : index);
		index = stack.size()-1 - index;

		auto value = stack[index];
		stack.erase(stack.begin() + index);
		stack.push_back(value);

		return stack.size() - index + 1;
	}
	return 1;
}

inline unsigned exec_yank(Env& env) {
	auto& stack = get_exec_stack(env);
	if (stack.size() >= 0 && get_stack<int>(env).size() > 0) {
		int index = pop<int>(env);
		index = index < 0 ? 0 : (index >= stack.size() ? stack.size()-1 : index);
		index = stack.size()-1 - index;

		auto value = stack[index];
		stack.erase(stack.begin() + index);
		stack.push_back(value);

		return stack.size() - index + 1;
	}
	return 1;
}

template <typename T>
inline unsigned yankdup(Env& env) {
	auto& stack = get_stack<T>(env);
	if (stack.size() >= 0 && get_stack<int>(env).size() > 0) {
		int index = pop<int>(env);
		index = index < 0 ? 0 : (index >= stack.size() ? stack.size()-1 : index);
		index = stack.size()-1 - index;

		stack.push_back(stack[index]);
	}
	return 1;
}

inline unsigned exec_yankdup(Env& env) {
	auto& stack = get_exec_stack(env);
	if (stack.size() >= 0 && get_stack<int>(env).size() > 0) {
		int index = pop<int>(env);
		index = index < 0 ? 0 : (index >= stack.size() ? stack.size()-1 : index);
		index = stack.size()-1 - index;

		stack.push_back(stack[index]);
	}
	return 1;
}

}

#endif // COMMON_OPS_H
