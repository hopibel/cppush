#include "code.h"
#include "common_ops.h"
#include "env.h"
#include "util.h"

#include <algorithm>

namespace cppush {

namespace detail {

unsigned code_equal_impl(Env& env, std::vector<Code>& stack) {
	if (stack.size() >= 2) {
		auto first = stack.back();
		stack.pop_back();
		auto second = stack.back();
		stack.pop_back();

		env.push<bool>(first == second);

		return std::visit(overloaded{
			[](CodeList& first, CodeList& second) -> unsigned {
				return std::min(first.size(), second.size()); },
			[](auto&&, auto&&) -> unsigned { return 1; }
		}, first, second);
	}
	return 1;
}

} // namespace detail

template <>
unsigned equal<Code>(Env& env) {return detail::code_equal_impl(env, env.get_stack<Code>());}
template <>
unsigned equal<Exec>(Env& env) {return detail::code_equal_impl(env, env.get_stack<Exec>());}

/**
 * input_n base case
 */
template <>
unsigned input_n<0>(Env& env) {
	return env.push_input_(0);
}

} // namespace cppush
