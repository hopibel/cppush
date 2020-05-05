#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include "code.h"
#include "instruction.h"
#include "types.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cppush {

namespace detail {

extern const std::map<std::string, std::pair<unsigned (*)(Env&), Types>> core_instructions;
void load_instruction(std::vector<Code>& instruction_set, std::string name);

} // namespace detail

void register_core(std::vector<Code>& instruction_set);

template <typename... Args>
void register_core_by_name(std::vector<Code>& instruction_set, Args... args) {
	(detail::load_instruction(instruction_set, args), ...);
}

// Append instructions for the enabled stacks
void register_core_by_stack(std::vector<Code>& instruction_set, const Types& types);

// Create input instructions
void register_n_inputs(std::vector<Code>& instruction_set, int n);

} // namespace cppush

#endif // INSTRUCTION_SET_H
