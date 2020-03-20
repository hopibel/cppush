#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <initializer_list>
#include <map>
#include <memory>
#include <string>

namespace cppush {

enum {
	EXEC = 1 << 0,
	CODE = 1 << 1,
	BOOL = 1 << 2,
	INT = 1 << 3,
	FLOAT = 1 << 4,
};

class Instruction;

class InstructionSet {
	public:
		// all core insns. TODO(hopibel): bitmask to filter which types get enabled
		void register_core();

		// register by data types used. ex: INT | BOOL
		void register_core(int flags);

		// register by name/s. std::initializer_list
		void register_core(std::string name) {register_core({name});}
		void register_core(std::initializer_list<std::string> names);

		// register a new instruction
		// TODO(hopibel): directly register shared_ptr<Instruction>? needs <memory> and "instruction.h"
		void register(unsigned (*op)(Env&), std::string name, int typemask, unsigned parens = 0); // register

	private:
		std::map<std::string, std::shared_ptr<Instruction>> insns_;
};

} // namespace cppush

#endif // INSTRUCTION_SET_H
