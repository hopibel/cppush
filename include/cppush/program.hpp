#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "literal_base.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace cppush {

struct Bytecode {
	std::uint8_t opcode;
	std::uint8_t arg;
};

// TODO: include interpreter settings
struct Program {
	std::vector<Bytecode> code; // main program
	std::vector<std::vector<Bytecode>> blocks; // nested code lists
	std::vector<std::shared_ptr<LiteralBase>> constant_pool;
};

} // namespace cppush

#endif // PROGRAM_HPP