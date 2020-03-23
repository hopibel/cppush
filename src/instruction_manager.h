#ifndef INSTRUCTION_MANAGER_H
#define INSTRUCTION_MANAGER_H

#include "instruction.h"

#include <map>
#include <memory>

namespace cppush {

class Instruction;

class InstructionManager {
	public:
		// all core insns
		void register_core();

		// register a new instruction
		// TODO(hopibel): directly register shared_ptr<Instruction>? needs <memory> and "instruction.h"
		void register_op(unsigned (*op)(Env&), std::string name, int typemask, unsigned parens = 0);

	private:
		std::map<std::string, std::shared_ptr<Instruction>> insns_;
};

} // namespace cppush

#endif // INSTRUCTION_MANAGER_H
