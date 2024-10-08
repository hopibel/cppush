#include "bool_ops.h"
#include "env.h"

namespace cppush {

unsigned bool_and(Env& env) {
	auto& stack = env.get_stack<bool>();
	if (stack.size() >= 2) {
		bool top = env.pop<bool>();
		stack.back() = stack.back() && top;
	}
	return 1;
}

unsigned bool_or(Env& env) {
	auto& stack = env.get_stack<bool>();
	if (stack.size() >= 2) {
		bool top = env.pop<bool>();
		stack.back() = stack.back() || top;
	}
	return 1;
}

unsigned bool_not(Env& env) {
	auto& stack = env.get_stack<bool>();
	if (stack.size() > 0) {
		stack.back() = !stack.back();
	}
	return 1;
}

unsigned bool_nand(Env& env) {
	auto& stack = env.get_stack<bool>();
	if (stack.size() >= 2) {
		bool top = env.pop<bool>();
		stack.back() = !(stack.back() && top);
	}
	return 1;
}

unsigned bool_nor(Env& env) {
	auto& stack = env.get_stack<bool>();
	if (stack.size() >= 2) {
		bool top = env.pop<bool>();
		stack.back() = !(stack.back() || top);
	}
	return 1;
}

unsigned bool_xor(Env& env) {
	auto& stack = env.get_stack<bool>();
	if (stack.size() >= 2) {
		bool top = env.pop<bool>();
		stack.back() = stack.back() != top;
	}
	return 1;
}

unsigned bool_invert_first_then_and(Env& env) {
	auto& stack = env.get_stack<bool>();
	if (stack.size() >= 2) {
		bool top = env.pop<bool>();
		stack.back() = stack.back() && !top;
	}
	return 1;
}

unsigned bool_invert_second_then_and(Env& env) {
	auto& stack = env.get_stack<bool>();
	if (stack.size() >= 2) {
		bool top = env.pop<bool>();
		stack.back() = !stack.back() && top;
	}
	return 1;
}

} // namespace cppush
