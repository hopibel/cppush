#ifndef INSTRUCTION_MANAGER_H
#define INSTRUCTION_MANAGER_H

#include "instruction.h"

#include <map>
#include <memory>
#include <vector>

namespace cppush {

class Instruction;

class InstructionManager {
	public:
		// all core insns
		void register_core();

		// register a new instruction
		// TODO(hopibel): directly register shared_ptr<Instruction>? needs <memory> and "instruction.h"
		void register_op(unsigned (*op)(Env&), std::string name, int typemask, unsigned parens = 0);

		// retrieve a vector of instructions that access any of the stacks in the bitmask (OR).
		// optionally exclude instructions based on a second bitmask
		std::vector<std::shared_ptr<Instruction>> get_by_stack(int bitmask, int exclude_mask = 0);

		// TODO(hopibel): get_by_name

	private:
		std::map<std::string, std::shared_ptr<Instruction>> insns_;
};

} // namespace cppush

#endif // INSTRUCTION_MANAGER_H
