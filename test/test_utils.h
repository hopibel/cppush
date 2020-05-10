#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "code.h"
#include "literal.h"

#include <vector>

template <typename T>
auto generate_test_values(int len) = delete;

template <>
auto generate_test_values<int>(int len) {
	std::vector<int> vec;
	for (int i = 0; i < len; ++i) {
		vec.push_back(i);
	}
	return vec;
}

template <>
auto generate_test_values<double>(int len) {
	std::vector<double> vec;
	for (double f = 0; f < len; ++f) {
		vec.push_back(f + 0.5);
	}
	return vec;
}

template <>
auto generate_test_values<bool>(int len) {
	std::vector<bool> vec;
	for (int i = 0; i < len; ++i) {
		vec.push_back(static_cast<bool>(i % 2));
	}
	return vec;
}

template <>
auto generate_test_values<cppush::Code_ptr>(int len) {
	std::vector<cppush::Code_ptr> vec;
	for (int i = 0; i < len; ++i) {
		vec.push_back(std::make_shared<cppush::Literal<int>>(i));
	}
	return vec;
}

template <>
auto generate_test_values<cppush::Exec>(int len) {
	return generate_test_values<cppush::Code_ptr>(len);
}

#endif // TEST_UTILS_H
