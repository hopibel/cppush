#include "cppush/state.hpp"

#include <algorithm>

namespace cppush {

void State::run(const std::vector<std::shared_ptr<Code>> prog) {
	exec_stack = prog;
	std::reverse(exec_stack.begin(), exec_stack.end());
	while (exec_stack.size() > 0) {
		auto op = exec_stack.back();
		exec_stack.pop_back();
		(*op)(*this);
	}
}

/* TODO: interesting approach, but having literally every instruction as a template is a little much
struct Exec {};
class Env {
public:
	std::vector<int> exec;
};

template <typename T, typename U>
auto& get_stack(U& env);

template <>
auto& get_stack<Exec>(Env& env) { return env.exec; }

template <typename T>
size_t exec_size(T& env) {
	return get_stack<Exec>(env).size();
}

class CustomEnv : public Env {
public:
	std::vector<int> ints;
};
template <> auto& get_stack<Exec>(CustomEnv& env) { return env.exec; }
*/

} // namespace cppush