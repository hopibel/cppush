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
		return op_ == static_cast<const Instruction&>(rhs).op_;
	}

private:
	unsigned (*const op_)(Env&);
};

} // namespace cppush

#endif // INSTRUCTION_H
