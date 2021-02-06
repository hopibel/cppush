#ifndef ENV_H
#define ENV_H

#include "code.h"

#include <algorithm>
#include <optional>
#include <vector>
#include <utility>

namespace cppush {

//struct Parameters {
//	int max_points_in_random_expressions{50};
//	int max_points{100};
//	int evalpush_limit{1000};
//	bool top_level_push_code{false};
//	bool top_level_pop_code{false};
//};

struct PushConfig {
	int inputs_expected = 0;
	int outputs_expected = 0;
};

// program's behavior depends on code and interpreter parameters
struct Program {
	Code code = CodeList();
	PushConfig config = PushConfig{};
};

struct Exec {};

class Env {
public:
	// returns computational effort
	int run(const Program& program, std::vector<Literal> inputs);

	// convert outputs from Literals to base types. result may be null
	template <typename T>
	std::vector<std::optional<T>> get_outputs() const;

	// needed for generic stack manipulation functions
	template <typename T> auto& get_stack() = delete;
	template <typename T> auto pop();
	template <typename T, typename U> void push(const U item);

private:
	// Stacks
	std::vector<Code> exec_stack;
	std::vector<Code> code_stack;
	std::vector<int> int_stack;
	std::vector<double> float_stack;
	std::vector<bool> bool_stack;
	// use inexact module tagging instead of names
	// TODO(hopibel): add a vector type for basic data types

	std::vector<Literal> inputs;
	std::vector<std::optional<Literal>> outputs;

	// push nth input item onto appropriate stack.
	// n is assumed to be a valid index
	template <int N> friend unsigned input_n(Env& env);
	void push_input(int n) { inputs[n](*this); }

	// set nth output to top item on T stack
	template <typename T, int N> friend unsigned output_n(Env& env);
	template <typename T> void set_output(int n, T value) { outputs[n] = value; }
};

template <> inline auto& Env::get_stack<int>() {return int_stack;}
template <> inline auto& Env::get_stack<double>() {return float_stack;}
template <> inline auto& Env::get_stack<bool>() {return bool_stack;}
template <> inline auto& Env::get_stack<Code>() {return code_stack;}
template <> inline auto& Env::get_stack<Exec>() {return exec_stack;}

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

template <typename T>
std::vector<std::optional<T>> Env::get_outputs() const {
	std::vector<std::optional<T>> vec;

	for (const auto& el : outputs) {
		if (el.has_value()) {
			auto value = el->get();
			if (std::holds_alternative<T>(value)) {
				vec.emplace_back(std::get<T>(value));
				continue;
			}
		}
		vec.emplace_back();
	}

	return vec;
}

} // namespace cppush

#endif // ENV_H
