#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "code.h"
#include "env.h"
#include "literal.h"

#include <memory>
#include <vector>

template <typename T>
void generate_test_values(cppush::Env&, int len);

template <> void generate_test_values<int>(cppush::Env& env, int len);
template <> void generate_test_values<double>(cppush::Env& env, int len);
template <> void generate_test_values<bool>(cppush::Env& env, int len);
template <> void generate_test_values<cppush::Code>(cppush::Env& env, int len);
template <> void generate_test_values<cppush::Exec>(cppush::Env& env, int len);

#endif // TEST_UTILS_H
