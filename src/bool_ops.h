#ifndef BOOL_OPS_H
#define BOOL_OPS_H

#include "env.h"

namespace cppush {

inline unsigned and_(Env& env) {
	auto stack = get_stack<bool>(env);
	if (stack.size() >= 2) {
		bool top = stack.back();
		stack.pop_back();

		stack.back() = stack.back() && top;
	}
	return 1;
}

inline unsigned or_(Env& env) {
	auto stack = get_stack<bool>(env);
	if (stack.size() >= 2) {
		bool top = stack.back();
		stack.pop_back();

		stack.back() = stack.back() || top;
	}
	return 1;
}

inline unsigned not_(Env& env) {
	auto stack = get_stack<bool>(env);
	if (stack.size() >= 1) {
		stack.back() = !stack.back();
	}
	return 1;
}

inline unsigned nand_(Env& env) {
	auto stack = get_stack<bool>(env);
	if (stack.size() >= 2) {
		bool top = stack.back();
		stack.pop_back();

		stack.back() = !(stack.back() && top);
	}
	return 1;
}

inline unsigned nor_(Env& env) {
	auto stack = get_stack<bool>(env);
	if (stack.size() >= 2) {
		bool top = stack.back();
		stack.pop_back();

		stack.back() = !(stack.back() || top);
	}
	return 1;
}

inline unsigned xor_(Env& env) {
	auto stack = get_stack<bool>(env);
	if (stack.size() >= 2) {
		bool top = stack.back();
		stack.pop_back();

		stack.back() = stack.back() != top;
	}
	return 1;
}

} // namespace cppush

#endif // BOOL_OPS_H
