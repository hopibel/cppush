#include "env.h"

#include <memory>
#include <vector>

namespace cppush {

void Env::load_program(const Code_ptr& program) {
	push<Exec>(program);
}

void Env::run() {
	// pop-execute loop
	while (get_stack<Exec>().size() > 0) {
		auto top = pop<Exec>();
		(*top)(*this);
	}
}

} // namespace cppush
