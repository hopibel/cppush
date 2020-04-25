#ifndef COMMON_OPS_H
#define COMMON_OPS_H

#include "code.h"
#include "env.h"

namespace cppush {

template <typename T>
inline unsigned equal(Env& env) {
	if (env.get_stack<T>().size() >= 2) {
		auto first = env.pop<T>();
		auto second = env.pop<T>();
		env.get_stack<bool>().push_back(first == second);
	}
	return 1;
}

namespace detail {
inline unsigned code_equal_impl(Env& env, std::vector<Code_ptr>& stack) {
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
inline unsigned equal<Code_ptr>(Env& env) {return detail::code_equal_impl(env, env.get_stack<Code_ptr>());}
inline unsigned exec_equal(Env& env) {return detail::code_equal_impl(env, env.get_exec_stack());}

template <typename T>
inline unsigned protected_pop(Env& env) {
	if (!env.get_stack<T>().empty()) {
		env.pop<T>();
	}
	return 1;
}

// can't template because exec_stack is same type as code_stack
inline unsigned protected_exec_pop(Env& env) {
	if (!env.get_exec_stack().empty()) {
		env.pop_exec();
	}
	return 1;
}

template <typename T>
inline unsigned dup(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() > 0) {
		stack.push_back(stack.back());
	}
	return 1;
}
inline unsigned exec_dup(Env& env) {
	auto& stack = env.get_exec_stack();
	if (stack.size() > 0) {
		stack.push_back(stack.back());
	}
	return 1;
}

template <typename T>
inline unsigned flush(Env& env) {
	env.get_stack<T>().clear();
	return 1;
}
inline unsigned exec_flush(Env& env) {
	env.get_exec_stack().clear();
	return 1;
}

template <typename T>
inline unsigned rot(Env& env) {
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
inline unsigned exec_rot(Env& env) {
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

template <typename T>
inline unsigned shove(Env& env) {
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
inline unsigned exec_shove(Env& env) {
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

template <typename T>
inline unsigned stackdepth(Env& env) {
	env.get_stack<int>().push_back(env.get_stack<T>().size());
	return 1;
}
inline unsigned exec_stackdepth(Env& env) {
	env.get_stack<int>().push_back(env.get_exec_stack().size());
	return 1;
}

template <typename T>
inline unsigned swap(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		auto first = env.pop<T>();
		auto second = env.pop<T>();
		stack.push_back(first);
		stack.push_back(second);
	}
	return 1;
}
inline unsigned exec_swap(Env& env) {
	auto& stack = env.get_exec_stack();
	if (stack.size() >= 2) {
		auto first = env.pop_exec();
		auto second = env.pop_exec();
		stack.push_back(first);
		stack.push_back(second);
	}
	return 1;
}

template <typename T>
inline unsigned yank(Env& env) {
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
inline unsigned exec_yank(Env& env) {
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

template <typename T>
inline unsigned yankdup(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 0 && env.get_stack<int>().size() > 0) {
		int index = env.pop<int>();
		index = index < 0 ? 0 : (index >= static_cast<int>(stack.size()) ? stack.size()-1 : index);
		index = stack.size()-1 - index;

		stack.push_back(stack[index]);
	}
	return 1;
}
inline unsigned exec_yankdup(Env& env) {
	auto& stack = env.get_exec_stack();
	if (stack.size() > 0 && env.get_stack<int>().size() > 0) {
		int index = env.pop<int>();
		index = index < 0 ? 0 : (index >= static_cast<int>(stack.size()) ? stack.size()-1 : index);
		index = stack.size()-1 - index;

		stack.push_back(stack[index]);
	}
	return 1;
}

}

#endif // COMMON_OPS_H
