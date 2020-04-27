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

class Env {
	public:
		// Stacks
		std::vector<std::shared_ptr<Code>> exec_stack;
		std::vector<std::shared_ptr<Code>> code_stack;
		std::vector<int> int_stack;
		std::vector<double> float_stack;
		std::vector<bool> bool_stack;
		// use inexact module tagging instead of names
		// TODO(hopibel): add a vector type for basic data types

		void load_program(const std::shared_ptr<Code>& program); // TODO(hopibel): Program struct with config
		void run();

		// needed for generic stack manipulation functions
		template <typename T> std::vector<T>& get_stack() = delete;
		// exec is just another Code_ptr stack. can't template it
		std::vector<Code_ptr>& get_exec_stack() {return exec_stack;};

		template <typename T> T pop();
		Code_ptr pop_exec() {return pop_impl_(get_exec_stack());};
	
	private:
		std::vector<std::shared_ptr<Code>> instruction_set_;

		// avoid duplicating code for exec functions
		template <typename T> T pop_impl_(std::vector<T>& stack);
};

template <> inline std::vector<int>& Env::get_stack() {return int_stack;}
template <> inline std::vector<double>& Env::get_stack() {return float_stack;}
template <> inline std::vector<bool>& Env::get_stack() {return bool_stack;}
template <> inline std::vector<Code_ptr>& Env::get_stack() {return code_stack;}

// pop from a stack and return element popped
template <typename T>
T Env::pop_impl_(std::vector<T>& stack) {
	T top = stack.back();
	stack.pop_back();
	return top;
}
template <typename T> T Env::pop() {return pop_impl_(get_stack<T>());}

} // namespace cppush

#endif // ENV_H
