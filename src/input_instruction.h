#ifndef INPUT_INSTRUCTION_H
#define INPUT_INSTRUCTION_H

#include "code.h"
#include "env.h"

#include <string>

namespace cppush {

class InputInstruction : public CodeAtom {
public:
	const std::string name;

	InputInstruction(std::string name, int index) :
		name(name), index_(index) {}

	unsigned operator()(Env& env) const override {
		return (*env.get_input_(index_))(env);
	}

protected:
	bool equal_to(const Code& rhs) const override {
		return name == static_cast<const InputInstruction&>(rhs).name;
	}

private:
	const int index_;
};

} // namespace cppush

#endif // INPUT_INSTRUCTION_H
