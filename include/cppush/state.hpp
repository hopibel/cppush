#ifndef STATE_HPP
#define STATE_HPP

#include "code.hpp"

#include <memory>
#include <vector>

namespace cppush {

class State {
public:
	State() {}
	void run(const std::vector<std::shared_ptr<Code>> program);

	template <typename T> auto& get_stack() = delete;
	template <typename T, typename U> void push(const U item);
	template <typename T> auto pop();

private:
	std::vector<std::shared_ptr<Code>> exec_stack;
	std::vector<double> number_stack;
};

struct Exec;
template <> inline auto& State::get_stack<Exec>() { return exec_stack; }
template <> inline auto& State::get_stack<double>() { return number_stack; }

template <typename T, typename U>
void State::push(const U item) {
	get_stack<T>().push_back(item);
}

template <typename T>
auto State::pop() {
	auto& stack = get_stack<T>();
	auto top = stack.back();
	stack.pop_back();
	return top;
}

} // namespace cppush

#endif // STATE_HPP