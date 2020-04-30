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
void load_instruction(std::vector<Code_ptr>& instruction_set, std::string name);

} // namespace detail

void register_core(std::vector<Code_ptr>& instruction_set);

template <typename... Args>
void register_core_by_name(std::vector<Code_ptr>& instruction_set, Args... args) {
	(detail::load_instruction(instruction_set, args), ...);
}

// TODO(hopibel): pass shared_ptrs by reference when possible
// check existing code for this

} // namespace cppush

#endif // INSTRUCTION_SET_H
