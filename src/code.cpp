#include "code.h"
#include "env.h"

#include <memory>
#include <typeinfo>

namespace cppush {

bool CodeBase::operator==(const CodeBase& rhs) const {
	const CodeBase& lhs = *this;

	if (&lhs == &rhs) {return true;}
	if (typeid(lhs) != typeid(rhs)) {return false;}
	
	auto lstack = lhs.get_stack();
	auto rstack = rhs.get_stack();

	if (lstack.size() != rstack.size()) {return false;}
	for (unsigned i = 0; i < lstack.size(); ++i) {
		if (!(*lstack[i] == *rstack[i])) {return false;}
	}

	return lhs.equal_to(rhs);
}

unsigned CodeList::operator()(Env& env) const {
	for (auto it = stack_.rbegin(); it < stack_.rend(); ++it) {
		env.push<Exec>(*it);
	}
	return 1;
}

void CodeList::calc_size_() {
	size_ = 1; // +1 for the list itself
	for (auto i : stack_) {
		size_ += i->size();
	}
}

} // namespace cppush
