#ifndef INSTRUCTION_SET_HPP
#define INSTRUCTION_SET_HPP

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace cppush {

class State;

class InstructionSet {
public:
	std::uint8_t get_opcode(std::string name) {
		for (std::uint8_t i = 0; i < names.size(); ++i) {
			if (names[i] == name) {
				return i;
			}
		}
		// TODO: what do if not found?
		throw std::invalid_argument("Unrecognized opcode: " + name);
	}

	auto get_op(std::string name) {
		return instructions[get_opcode(name)];
	}

private:
	std::vector<std::string> names;
	std::vector<void (*)(State&)> instructions;
};

//void register_core(std::vector<Instruction>& instruction_set);
//std::vector<Instruction> register_core();

//void register_core_by_name(std::vector<Instruction>& instruction_set, std::vector<std::string> names);
//std::vector<Instruction> register_core_by_name(std::vector<std::string> names);

// Append instructions for the enabled stacks
//void register_core_by_stack(std::vector<Instruction>& instruction_set, const Types& types);
//std::vector<Instruction> register_core_by_stack(const Types& types);

/**
 * Uses recursive templates to generate the functions at compile time.
 * This lets us use a vector of Instructions and avoid polymorphism.
 */
//template <int N>
//void register_n_inputs(std::vector<Instruction>& instruction_set) {
//	static_assert(N > 0, "register_n_inputs(): N must be > 0");
//
//	if constexpr (N > 1) {
//		register_n_inputs<N-1>(instruction_set);
//	}
//	instruction_set.emplace_back(input_n<N-1>, "input_" + std::to_string(N-1));
//}

// convenience function. return a new vector instead of appending to one
//template <int N>
//std::vector<Instruction> register_n_inputs() {
//	std::vector<Instruction> inputs;
//	register_n_inputs<N>(inputs);
//	return inputs;
//}

// generate output instructions for one type. mixed type must be done manually
//template <typename T, int N>
//void register_n_outputs(std::vector<Instruction>& instruction_set) {
//	static_assert(N > 0, "register_n_outputs(): N must be > 0");
//
//	if constexpr (N > 1) {
//		register_n_outputs<T, N-1>(instruction_set);
//	}
//	instruction_set.emplace_back(output_n<T, N-1>, "output_" + std::to_string(N-1));
//}

} // namespace cppush

#endif // INSTRUCTION_SET_HPP
