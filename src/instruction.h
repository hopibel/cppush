#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "code.h"
#include "env.h"

#include <memory>
#include <ostream>
#include <string>

namespace cppush {

class Instruction : public CodeAtom {
	private:
		unsigned (*const op_)(Env&);

	public:
		const std::string name;
		const int types;
		const unsigned parens;

		Instruction(unsigned (*op)(Env&), std::string name, int typemask, unsigned parens = 0) :
			op_(op), name(name), types(typemask), parens(parens) {}

		unsigned operator()(Env& env) const override {return op_(env);}

	protected:
		bool equal_to(const Code& rhs) const override {
			return op_ == static_cast<const Instruction&>(rhs).op_;
		}
};

// std::ostream& operator<<(std::ostream& o, const Instruction& insn) {
// 	return o << insn.to_string();
// }
// 
// std::ostream& operator<<(std::ostream& o, const std::shared_ptr<Instruction>& insn) {
// 	return o << insn->to_string();
// }

} // namespace cppush

#endif // INSTRUCTION_H
