#ifndef ENV_H
#define ENV_H

#include "code.h"

#include <chrono>
#include <memory>
#include <vector>

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

// TODO(hopibel): move to code.h ?
struct Exec {};

class Env {
public:
	void load_program(const Code_ptr& program); // TODO(hopibel): Program struct with config
	template <typename T> void load_inputs(std::vector<T> inputs);

	void run();

	// needed for generic stack manipulation functions
	template <typename T> auto& get_stack() = delete;
	template <typename T> auto pop();
	template <typename T, typename U> inline void push(const U& item);

private:
	std::vector<Code_ptr> instruction_set_;

	// Stacks
	std::vector<Code_ptr> exec_stack_;
	std::vector<Code_ptr> code_stack_;
	std::vector<int> int_stack_;
	std::vector<double> float_stack_;
	std::vector<bool> bool_stack_;
	// use inexact module tagging instead of names
	// TODO(hopibel): add a vector type for basic data types

	// Input values
	std::vector<Code_ptr> inputs_;

	// retrieve nth input item. needed for implementing input instructions.
	// n is assumed to be a valid index
	friend class InputInstruction;
	Code_ptr get_input_(int n) {return inputs_[n];}
};

} // namespace cppush

#include "literal.h"

// definitions
namespace cppush {

template <typename T>
void Env::load_inputs(const std::vector<T> inputs) {
	inputs_.clear();
	for (const auto el : inputs) {
		inputs_.push_back(std::make_shared<Literal<T>>(el));
	}
}

template <> inline auto& Env::get_stack<int>() {return int_stack_;}
template <> inline auto& Env::get_stack<double>() {return float_stack_;}
template <> inline auto& Env::get_stack<bool>() {return bool_stack_;}
template <> inline auto& Env::get_stack<Code_ptr>() {return code_stack_;}
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
inline void Env::push(const U& item) { get_stack<T>().push_back(item); }
template <>
inline void Env::push<Exec>(const Code_ptr& item) { get_stack<Exec>().push_back(item); }

} // namespace cppush

#endif // ENV_H
