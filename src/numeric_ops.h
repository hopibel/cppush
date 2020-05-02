#ifndef NUMERIC_OPS_H
#define NUMERIC_OPS_H

#include "env.h"

#include <cmath>
#include <vector>

namespace cppush {

template <typename T>
unsigned add(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		T top = env.pop<T>();
		stack.back() += top;
	}
	return 1;
}

template <typename T>
unsigned sub(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		T top = env.pop<T>();
		stack.back() -= top;
	}
	return 1;
}

template <typename T>
unsigned mul(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		T top = env.pop<T>();
		stack.back() *= top;
	}
	return 1;
}

template <typename T>
unsigned div(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2 && stack.back() != T(0)) {
		T top = env.pop<T>();
		stack.back() /= top;
	}
	return 1;
}

unsigned int_mod(Env& env);
unsigned float_mod(Env& env);

template <typename T>
unsigned lt(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		T top = env.pop<T>();
		T second = env.pop<T>();

		env.get_stack<bool>().push_back(second < top);
	}
	return 1;
}

template <typename T>
unsigned gt(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		T top = env.pop<T>();
		T second = env.pop<T>();

		env.get_stack<bool>().push_back(second > top);
	}
	return 1;
}

unsigned int_from_bool(Env& env);
unsigned int_from_float(Env& env);
unsigned float_from_bool(Env& env);
unsigned float_from_int(Env& env);
unsigned bool_from_int(Env& env);
unsigned bool_from_float(Env& env);

template <typename T>
unsigned max(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		T top = env.pop<T>();

		if (stack.back() < top) {
			stack.back() = top;
		}
	}
	return 1;
}

template <typename T>
unsigned min(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		T top = env.pop<T>();

		if (stack.back() > top) {
			stack.back() = top;
		}
	}
	return 1;
}

unsigned trig_cos(Env& env);
unsigned trig_sin(Env& env);
unsigned trig_tan(Env& env);

} // namespace cppush

#endif // NUMERIC_OPS_H
