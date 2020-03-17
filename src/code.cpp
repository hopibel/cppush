#include "code.h"
#include "env.h"

#include <memory>
#include <typeinfo>

namespace cppush {

bool Code::operator==(const Code& rhs) const {
	const Code& lhs = *this;

	if (&lhs == &rhs) {return true;}
	if (typeid(lhs) != typeid(rhs)) {return false;}
	
	auto lstack = lhs.get_stack();
	auto rstack = rhs.get_stack();

	if (lstack.size() != rstack.size()) {return false;}
	for (int i = 0; i < lstack.size(); ++i) {
		if (!(*lstack[i] == *rstack[i])) {return false;}
	}

	return lhs.equal_to(rhs);
}

unsigned CodeList::operator()(Env& env) const {
	for (auto it = stack_.rbegin(); it < stack_.rend(); ++it) {
		env.exec_stack.push_back(*it);
	}
	return 1;
}

// TODO(hopibel): can probably cache this and force recalculate on modification
unsigned CodeList::size() const {
	unsigned size = 1; // +1 for the list itself
	for (auto i : stack_) {
		size += i->size();
	}
	return size;
}

} // namespace cppush
