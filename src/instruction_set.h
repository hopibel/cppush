#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include "code.h"
#include "instruction.h"
#include "types.h"

#include <initializer_list>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cppush {

void register_core(std::vector<Code_ptr>& instruction_set);

void register_core_by_name(std::vector<Code_ptr>& instruction_set, std::initializer_list<std::string> names);

// Append instructions for the enabled stacks
void register_core_by_stack(std::vector<Code_ptr>& instruction_set, const Types& types);

// Create input instructions
void register_n_inputs(std::vector<Code_ptr>& instruction_set, int n);

} // namespace cppush

#endif // INSTRUCTION_SET_H
