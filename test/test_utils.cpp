#include "test_utils.h"
#include "code.h"
#include "env.h"
#include "literal.h"

#include <memory>
#include <vector>

template <>
void generate_test_values<int>(cppush::Env& env, int len) {
	for (int i = 0; i < len; ++i) {
		env.push<int>(i);
	}
}

template <>
void generate_test_values<double>(cppush::Env& env, int len) {
	for (double f = 0; f < len; ++f) {
		env.push<double>(f + 0.5);
	}
}

template <>
void generate_test_values<bool>(cppush::Env& env, int len) {
	for (int i = 0; i < len; ++i) {
		env.push<bool>(i % 2);
	}
}

template <>
void generate_test_values<cppush::Code>(cppush::Env& env, int len) {
	static std::vector<std::unique_ptr<cppush::Code>> guard;

	for (int i = 0; i < len; ++i) {
		guard.push_back(std::make_unique<cppush::Literal<int>>(i));
		env.push<cppush::Code>(guard.back().get());
	}
}

template <>
void generate_test_values<cppush::Exec>(cppush::Env& env, int len) {
	generate_test_values<cppush::Code>(env, len);
	auto& code_stack = env.get_stack<cppush::Code>();
	for (auto el : code_stack) {
		env.push<cppush::Exec>(el);
	}
	code_stack.clear();
}
