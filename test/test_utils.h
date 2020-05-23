#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "code.h"
#include "env.h"

#include <string>
#include <vector>

template <typename T>
auto generate_test_values(int len) = delete;

template <> inline auto generate_test_values<int>(int len) {
	std::vector<int> vec;
	for (int i = 0; i < len; ++i) {
		vec.push_back(i);
	}
	return vec;
}

template <> inline auto generate_test_values<double>(int len) {
	std::vector<double> vec;
	for (double i = 0; i < len; ++i) {
		vec.push_back(i + 0.5);
	}
	return vec;
}

template <> inline auto generate_test_values<bool>(int len) {
	std::vector<bool> vec;
	for (int i = 0; i < len; ++i) {
		vec.push_back(i % 2);
	}
	return vec;
}

template <> inline auto generate_test_values<cppush::Code>(int len) {
	std::vector<cppush::Code> vec;
	for (int i = 0; i < len; ++i) {
		vec.push_back(cppush::Literal(i));
	}
	return vec;
}

template <> inline auto generate_test_values<cppush::Exec>(int len) {
	return generate_test_values<cppush::Code>(len);
}

// string conversion for code vectors
inline std::ostream& operator<<(std::ostream& os, const std::vector<cppush::Code>& code) {
	os << std::string{"{ "};
	for (auto& el : code) {
		if (&el != &code[0]) {
			os << std::string{", "};
		}
		os << el;
	}
	os << std::string{" }"};
	return os;
}

#endif // TEST_UTILS_H
