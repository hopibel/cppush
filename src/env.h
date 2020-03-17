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
	
	private:
		std::vector<std::shared_ptr<Code>> instruction_set_;
};

template <typename T> inline std::vector<T>& get_stack(Env& env) {return get_stack<T>(env);}
template <> inline std::vector<std::shared_ptr<Code>>& get_stack(Env& env) {return env.code_stack;}
template <> inline std::vector<int>& get_stack(Env& env) {return env.int_stack;}
template <> inline std::vector<double>& get_stack(Env& env) {return env.float_stack;}
template <> inline std::vector<bool>& get_stack(Env& env) {return env.bool_stack;}
inline std::vector<std::shared_ptr<Code>>& get_exec_stack(Env& env) {return env.exec_stack;}
// implement exec_stack functions manually

template <typename T> inline T pop(Env& env) {
	T first = get_stack<T>(env).back();
	get_stack<T>(env).pop_back();
	return first;
}
inline std::shared_ptr<Code> pop_exec(Env& env) {
	auto first = get_exec_stack(env).back();
	get_exec_stack(env).pop_back();
	return first;
}

} // namespace cppush

#endif // ENV_H
