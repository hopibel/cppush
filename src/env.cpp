#include "code.h"
#include "env.h"

#include <stdexcept>
#include <variant>
#include <vector>

namespace cppush {

int Env::run(const Program& program, std::vector<Literal> inputs) {
	// validate input length
	if (static_cast<int>(inputs.size()) != program.config.inputs_expected) {
		throw std::length_error("Env::run(): inputs don't match number expected");
	}

	// initialize output vector
	outputs = std::vector<std::optional<Literal>>(program.config.outputs_expected);

	this->inputs = inputs;
	// push inputs in reverse onto appropriate stacks
	for (auto it = inputs.rbegin(); it < inputs.rend(); ++it) {
		(*it)(*this);
	}

	push<Exec>(program.code);
	push<Code>(program.code); // allow access to program's source via Code stack

	int effort = 0;

	while (get_stack<Exec>().size() > 0) {
		std::visit(
			[&](auto&& arg) { arg(*this); },
			pop<Exec>()
		);
	}

	return effort;
}

} // namespace cppush
