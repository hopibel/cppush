#include "cppush/number_ops.hpp"
#include "cppush/state.hpp"

#include <algorithm>
#include <cmath>

namespace cppush {

unsigned number_add(State& state) {
	if (state.get_stack<double>().size() >= 2) {
		double top = state.pop<double>();
		state.get_stack<double>().back() += top;
	}
	return 1;
}

unsigned number_sub(State& state) {
	if (state.get_stack<double>().size() >= 2) {
		double top = state.pop<double>();
		state.get_stack<double>().back() -= top;
	}
	return 1;
}

unsigned number_mul(State& state) {
	if (state.get_stack<double>().size() >= 2) {
		double top = state.pop<double>();
		state.get_stack<double>().back() *= top;
	}
	return 1;
}

unsigned number_div(State& state) {
	if (state.get_stack<double>().size() >= 2) {
		double top = state.pop<double>();
		state.get_stack<double>().back() /= top;
	}
	return 1;
}

unsigned number_mod(State& state) {
	if (state.get_stack<double>().size() >= 2) {
		double b = state.pop<double>();
		double a = state.pop<double>();
		state.push<double>(std::fmod(a, b));
	}
	return 1;
}

unsigned number_max(State& state) {
	if (state.get_stack<double>().size() >= 2) {
		double b = state.pop<double>();
		double a = state.pop<double>();
		state.push<double>(std::max(a, b));
	}
	return 1;
}

unsigned number_min(State& state) {
	if (state.get_stack<double>().size() >= 2) {
		double b = state.pop<double>();
		double a = state.pop<double>();
		state.push<double>(std::min(a, b));
	}
	return 1;
}

unsigned number_cos(State& state) {
	if (state.get_stack<double>().size() >= 1) {
		double top = state.pop<double>();
		state.push<double>(std::cos(top));
	}
	return 1;
}

unsigned number_sin(State& state) {
	if (state.get_stack<double>().size() >= 1) {
		double top = state.pop<double>();
		state.push<double>(std::sin(top));
	}
	return 1;
}

unsigned number_tan(State& state) {
	if (state.get_stack<double>().size() >= 1) {
		double top = state.pop<double>();
		state.push<double>(std::tan(top));
	}
	return 1;
}

} // namespace cppush