#ifndef ENV_H
#define ENV_H

#include "code.h"

#include <chrono>
#include <memory>
#include <vector>
#include <utility>

namespace cppush {

// TODO(hopibel): most of these are PushGP parameters, not for the Interpreter itself.
// consider pyshgp's ProgramSignature approach to reproducible execution
struct Parameters {
	int min_random_integer{-10};
	int max_random_integer{10};
	float min_random_float{-1.0};
	float max_random_float{1.0};
	int max_points_in_random_expressions{50};
	int max_points{100};
	int evalpush_limit{1000};
	long random_seed{std::chrono::high_resolution_clock::now().time_since_epoch().count()};
	bool top_level_push_code{false};
	bool top_level_pop_code{false};
};

// program's behavior depends on code and interpreter parameters
struct Program {
	CodeList code;
	// TODO: PushConfig
};

struct Exec {};

class Env {
public:
	void load_program(const Program& program, std::vector<Literal>&& inputs);
	void run();

	// needed for generic stack manipulation functions
	template <typename T> auto& get_stack() = delete;
	template <typename T> auto pop();
	template <typename T, typename U> void push(const U item);

private:
	// Stacks
	std::vector<Code> exec_stack_;
	std::vector<Code> code_stack_;
	std::vector<int> int_stack_;
	std::vector<double> float_stack_;
	std::vector<bool> bool_stack_;
	// use inexact module tagging instead of names
	// TODO(hopibel): add a vector type for basic data types

	// Input values
	std::vector<Literal> inputs_;

	// retrieve nth input item. needed for implementing input instructions.
	// n is assumed to be a valid index
	template <unsigned N> friend unsigned input_n(Env& env);
	unsigned push_input_(int n) {return inputs_[n](*this);}
};

template <> inline auto& Env::get_stack<int>() {return int_stack_;}
template <> inline auto& Env::get_stack<double>() {return float_stack_;}
template <> inline auto& Env::get_stack<bool>() {return bool_stack_;}
template <> inline auto& Env::get_stack<Code>() {return code_stack_;}
template <> inline auto& Env::get_stack<Exec>() {return exec_stack_;}

template <typename T>
inline auto Env::pop() {
	auto& stack = get_stack<T>();
	auto top = stack.back();
	stack.pop_back();
	return top;
}

// push to a stack
template <typename T, typename U>
inline void Env::push(const U item) { get_stack<T>().push_back(item); }
template <>
inline void Env::push<Exec>(const Code item) { get_stack<Exec>().push_back(item); }

} // namespace cppush

#endif // ENV_H
