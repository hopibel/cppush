#include "env.h"
#include "numeric_ops.h"

#include <cmath>
#include <vector>

namespace cppush {

unsigned int_mod(Env& env) {
	auto& stack = env.get_stack<int>();
	if (stack.size() >= 2 && stack.back() != 0) {
		int top = env.pop<int>();
		stack.back() %= top;
	}
	return 1;
}

unsigned float_mod(Env& env) {
	auto& stack = env.get_stack<double>();
	if (stack.size() >= 2 && stack.back() != 0.0) {
		double top = env.pop<double>();
		double second = env.pop<double>();

		stack.push_back(std::fmod(second, top));
	}
	return 1;
}

namespace detail {

template <typename From, typename To>
unsigned push_cast(Env& env) {
	auto& stack = env.get_stack<From>();
	if (stack.size() > 0) {
		From top = env.pop<From>();

		env.get_stack<To>().push_back(static_cast<To>(top));
	}
	return 1;
}

} // namespace detail

unsigned int_from_bool(Env& env) {return detail::push_cast<bool, int>(env);}
unsigned int_from_float(Env& env) {return detail::push_cast<double, int>(env);}
unsigned float_from_bool(Env& env) {return detail::push_cast<bool, double>(env);}
unsigned float_from_int(Env& env) {return detail::push_cast<int, double>(env);}
unsigned bool_from_int(Env& env) {return detail::push_cast<int, bool>(env);}
unsigned bool_from_float(Env& env) {return detail::push_cast<double, bool>(env);}

unsigned trig_cos(Env& env) {
	auto& stack = env.get_stack<double>();
	if (stack.size() > 0) {
		stack.back() = std::cos(stack.back());
	}
	return 1;
}

unsigned trig_sin(Env& env) {
	auto& stack = env.get_stack<double>();
	if (stack.size() > 0) {
		stack.back() = std::sin(stack.back());
	}
	return 1;
}

unsigned trig_tan(Env& env) {
	auto& stack = env.get_stack<double>();
	if (stack.size() > 0) {
		stack.back() = std::tan(stack.back());
	}
	return 1;
}

} // namespace cppush

