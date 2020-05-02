#include "code.h"
#include "common_ops.h"
#include "env.h"

#include <algorithm>

namespace cppush {

namespace detail {

unsigned code_equal_impl(Env& env, std::vector<Code_ptr>& stack) {
	if (stack.size() >= 2) {
		auto first = stack.back();
		stack.pop_back();
		auto second = stack.back();
		stack.pop_back();

		env.push<bool>(*first == *second);

		return std::min(first->size(), second->size());
	}
	return 1;
}

} // namespace detail

template <>
unsigned equal<Code_ptr>(Env& env) {return detail::code_equal_impl(env, env.get_stack<Code_ptr>());}
template <>
unsigned equal<Exec>(Env& env) {return detail::code_equal_impl(env, env.get_stack<Exec>());}

} // namespace cppush
