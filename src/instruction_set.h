#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include "code.h"
#include "common_ops.h"
#include "types.h"

#include <initializer_list>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cppush {

void register_core(std::vector<Instruction>& instruction_set);
std::vector<Instruction> register_core();

void register_core_by_name(std::vector<Instruction>& instruction_set, std::initializer_list<std::string> names);
std::vector<Instruction> register_core_by_name(std::initializer_list<std::string> names);

// Append instructions for the enabled stacks
void register_core_by_stack(std::vector<Instruction>& instruction_set, const Types& types);
std::vector<Instruction> register_core_by_stack(const Types& types);

/**
 * Uses recursive templates to generate the functions at compile time.
 * This lets us use a vector of Instructions and avoid polymorphism.
 */
template <unsigned N>
void register_n_inputs(std::vector<Instruction>& instruction_set) {
	static_assert(N > 0, "register_n_inputs template argument must be positive");

	register_n_inputs<N-1>(instruction_set);
	instruction_set.push_back(Instruction(input_n<N-1>, "input_" + (N-1)));
	return;
}
// base case
template <> void register_n_inputs<1>(std::vector<Instruction>& instruction_set);

template <unsigned N>
std::vector<Instruction> register_n_inputs() {
	std::vector<Instruction> inputs;
	register_n_inputs<N>(inputs);
	return inputs;
}

} // namespace cppush

#endif // INSTRUCTION_SET_H
