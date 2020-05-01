#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "code.h"
#include "env.h"

#include <memory>
#include <string>

namespace cppush {

class Instruction : public CodeAtom {
public:
	const std::string name;

	Instruction(unsigned (*op)(Env&), std::string name) :
		name(name), op_(op) {}

	unsigned operator()(Env& env) const override {return op_(env);}

protected:
	bool equal_to(const Code& rhs) const override {
		return name == static_cast<const Instruction&>(rhs).name;
	}

private:
	unsigned (*const op_)(Env&);
};

using Instruction_ptr = std::shared_ptr<Instruction>;

} // namespace cppush

#endif // INSTRUCTION_H
