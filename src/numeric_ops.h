#ifndef NUMERIC_OPS_H
#define NUMERIC_OPS_H

#include "env.h"

#include <cmath>
#include <vector>

namespace cppush {

template <typename T>
inline unsigned add(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		T top = env.pop<T>();
		stack.back() += top;
	}
	return 1;
}

template <typename T>
inline unsigned sub(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		T top = env.pop<T>();
		stack.back() -= top;
	}
	return 1;
}

template <typename T>
inline unsigned mul(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		T top = env.pop<T>();
		stack.back() *= top;
	}
	return 1;
}

template <typename T>
inline unsigned div(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2 && stack.back() != T(0)) {
		T top = env.pop<T>();
		stack.back() /= top;
	}
	return 1;
}

inline unsigned int_mod(Env& env) {
	auto& stack = env.get_stack<int>();
	if (stack.size() >= 2 && stack.back() != 0) {
		int top = env.pop<int>();
		stack.back() %= top;
	}
	return 1;
}

inline unsigned float_mod(Env& env) {
	auto& stack = env.get_stack<double>();
	if (stack.size() >= 2 && stack.back() != 0.0) {
		double top = env.pop<double>();
		double second = env.pop<double>();

		stack.push_back(std::fmod(second, top));
	}
	return 1;
}

template <typename T>
inline unsigned less_than(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		T top = env.pop<T>();
		T second = env.pop<T>();

		env.get_stack<bool>().push_back(second < top);
	}
	return 1;
}

template <typename T>
inline unsigned greater_than(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		T top = env.pop<T>();
		T second = env.pop<T>();

		env.get_stack<bool>().push_back(second > top);
	}
	return 1;
}

// TODO(hopibel): move to helper library
namespace detail {

template <typename From, typename To>
inline unsigned push_cast(Env& env) {
	auto& stack = env.get_stack<From>();
	if (stack.size() > 0) {
		From top = env.pop<From>();

		env.get_stack<To>().push_back(static_cast<To>(top));
	}
	return 1;
}

} // namespace detail

inline unsigned int_from_bool(Env& env) {return detail::push_cast<bool, int>(env);}
inline unsigned int_from_float(Env& env) {return detail::push_cast<double, int>(env);}
inline unsigned float_from_bool(Env& env) {return detail::push_cast<bool, double>(env);}
inline unsigned float_from_int(Env& env) {return detail::push_cast<int, double>(env);}
inline unsigned bool_from_int(Env& env) {return detail::push_cast<int, bool>(env);}
inline unsigned bool_from_float(Env& env) {return detail::push_cast<double, bool>(env);}

template <typename T>
inline unsigned max(Env& env) {
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
inline unsigned min(Env& env) {
	auto& stack = env.get_stack<T>();
	if (stack.size() >= 2) {
		T top = env.pop<T>();

		if (stack.back() > top) {
			stack.back() = top;
		}
	}
	return 1;
}

inline unsigned trig_cos(Env& env) {
	auto& stack = env.get_stack<double>();
	if (stack.size() > 0) {
		stack.back() = std::cos(stack.back());
	}
	return 1;
}

inline unsigned trig_sin(Env& env) {
	auto& stack = env.get_stack<double>();
	if (stack.size() > 0) {
		stack.back() = std::sin(stack.back());
	}
	return 1;
}

inline unsigned trig_tan(Env& env) {
	auto& stack = env.get_stack<double>();
	if (stack.size() > 0) {
		stack.back() = std::tan(stack.back());
	}
	return 1;
}

} // namespace cppush

#endif // NUMERIC_OPS_H
