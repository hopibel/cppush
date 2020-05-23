#include "env.h"

#include <memory>
#include <variant>
#include <vector>

namespace cppush {

void Env::load_program(const Program& program, std::vector<Literal>&& inputs) {
	inputs_ = inputs;
	push<Exec>(program.code);
}

void Env::run() {
	// pop-execute loop
	while (get_stack<Exec>().size() > 0) {
		std::visit(
			[&](auto&& arg) { arg(*this); },
			pop<Exec>()
		);
	}
}

} // namespace cppush
