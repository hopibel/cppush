#include "env.h"

#include <memory>
#include <vector>

namespace cppush {

void Env::load_program(const std::shared_ptr<Code>& program) {
	exec_stack.push_back(program);
}

void Env::run() {
	// pop-execute loop
	while (exec_stack.size() > 0) {
		std::shared_ptr<Code> top = exec_stack.back();
		exec_stack.pop_back();

		(*top)(*this);
	}
}

} // namespace cppush
