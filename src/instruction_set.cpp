#include "instruction_set.h"

#include "code.h"
#include "instruction.h"
#include "bool_ops.h"
#include "code_ops.h"
#include "common_ops.h"
#include "exec_ops.h"
#include "numeric_ops.h"
#include "types.h"

#include <utility>
#include <string>
#include <unordered_map>

namespace cppush {

namespace detail {

const std::map<std::string, std::pair<unsigned (*)(Env&), Types>> core_instructions{
	// exec
	{"EXEC.DO*RANGE", {exec_do_range, Type::EXEC | Type::INT}},
	{"EXEC.DO*COUNT", {exec_do_count, Type::EXEC | Type::INT}},
	{"EXEC.DO*TIMES", {exec_do_times, Type::EXEC | Type::INT}},
	{"EXEC.IF", {exec_if, Type::EXEC | Type::BOOL}},
	{"EXEC.K", {exec_k, Type::EXEC}},
	{"EXEC.S", {exec_s, Type::EXEC}},
	{"EXEC.Y", {exec_y, Type::EXEC}},

	// code
	{"CODE.APPEND", {code_append, Type::CODE}},
	{"CODE.ATOM", {code_atom, Type::CODE}},
	{"CODE.CAR", {code_car, Type::CODE}},
	{"CODE.CDR", {code_cdr, Type::CODE}},
	{"CODE.CONS", {code_cons, Type::CODE}},
	{"CODE.CONTAINER", {code_container, Type::CODE}},
	{"CODE.CONTAINS", {code_contains, Type::CODE | Type::BOOL}},
	{"CODE.DO", {code_do, Type::CODE | Type::EXEC}},
	{"CODE.DO*STAR", {code_do_star, Type::CODE | Type::EXEC}},
	{"CODE.QUOTE", {code_quote, Type::CODE | Type::EXEC}},
	{"CODE.DO*RANGE", {code_do_range, Type::CODE | Type::EXEC | Type::INT}},
	{"CODE.DO*COUNT", {code_do_count, Type::CODE | Type::EXEC | Type::INT}},
	{"CODE.DO*TIMES", {code_do_times, Type::CODE | Type::EXEC | Type::INT}},
	{"CODE.EXTRACT", {code_extract, Type::CODE | Type::INT}},
	{"CODE.FROMBOOL", {code_from_bool, Type::CODE | Type::BOOL}},
	{"CODE.FROMFLOAT", {code_from_float, Type::CODE | Type::FLOAT}},
	{"CODE.FROMINT", {code_from_int, Type::CODE | Type::INT}},
	{"CODE.IF", {code_if, Type::CODE | Type::BOOL | Type::EXEC}},
	{"CODE.INSERT", {code_insert, Type::CODE | Type::INT}},
	{"CODE.LENGTH", {code_length, Type::CODE | Type::INT}},
	{"CODE.LIST", {code_list, Type::CODE}},
	{"CODE.MEMBER", {code_member, Type::CODE | Type::BOOL}},
	{"CODE.NOOP", {code_noop, Type(0)}},
	{"CODE.NTH", {code_nth, Type::CODE | Type::INT}},
	{"CODE.NTHCDR", {code_nthcdr, Type::CODE | Type::INT}},
	{"CODE.NULL", {code_null, Type::CODE | Type::BOOL}},
	{"CODE.POSITION", {code_position, Type::CODE | Type::INT}},
	{"CODE.SIZE", {code_size, Type::CODE | Type::INT}},
	{"CODE.SUBST", {code_subst, Type::CODE}},

	// bool
	{"BOOLEAN.AND", {bool_and, Type::BOOL}},
	{"BOOLEAN.OR", {bool_or, Type::BOOL}},
	{"BOOLEAN.NOT", {bool_not, Type::BOOL}},
	{"BOOLEAN.NAND", {bool_nand, Type::BOOL}},
	{"BOOLEAN.NOR", {bool_nor, Type::BOOL}},
	{"BOOLEAN.XOR", {bool_xor, Type::BOOL}},
	{"BOOLEAN.INVERT_FIRST_THEN_AND", {bool_invert_first_then_and, Type::BOOL}},
	{"BOOLEAN.INVERT_SECOND_THEN_AND", {bool_invert_second_then_and, Type::BOOL}},
	{"BOOLEAN.FROMINT", {bool_from_int, Type::BOOL | Type::INT}},
	{"BOOLEAN.FROMFLOAT", {bool_from_float, Type::BOOL | Type::FLOAT}},

	// int
	{"INTEGER.+", {add<int>, Type::INT}},
	{"INTEGER.-", {sub<int>, Type::INT}},
	{"INTEGER.*", {mul<int>, Type::INT}},
	{"INTEGER./", {div<int>, Type::INT}},
	{"INTEGER.%", {int_mod, Type::INT}},
	{"INTEGER.<", {less_than<int>, Type::INT | Type::BOOL}},
	{"INTEGER.>", {greater_than<int>, Type::INT | Type::BOOL}},
	{"INTEGER.FROMBOOLEAN", {int_from_bool, Type::INT | Type::BOOL}},
	{"INTEGER.FROMFLOAT", {int_from_float, Type::INT | Type::FLOAT}},
	{"INTEGER.MAX", {max<int>, Type::INT}},
	{"INTEGER.MIN", {min<int>, Type::INT}},

	// float
	{"FLOAT.+", {add<double>, Type::FLOAT}},
	{"FLOAT.-", {sub<double>, Type::FLOAT}},
	{"FLOAT.*", {mul<double>, Type::FLOAT}},
	{"FLOAT./", {div<double>, Type::FLOAT}},
	{"FLOAT.%", {float_mod, Type::FLOAT}},
	{"FLOAT.<", {less_than<double>, Type::FLOAT | Type::BOOL}},
	{"FLOAT.>", {greater_than<double>, Type::FLOAT | Type::BOOL}},
	{"FLOAT.FROMBOOLEAN", {float_from_bool, Type::FLOAT | Type::BOOL}},
	{"FLOAT.FROMINTEGER", {float_from_int, Type::FLOAT | Type::INT}},
	{"FLOAT.MAX", {max<double>, Type::FLOAT}},
	{"FLOAT.MIN", {min<double>, Type::FLOAT}},
	{"FLOAT.COS", {trig_cos, Type::FLOAT}},
	{"FLOAT.SIN", {trig_sin, Type::FLOAT}},
	{"FLOAT.TAN", {trig_tan, Type::FLOAT}},

	// common stack ops
	{"CODE.=", {equal<Code_ptr>, Type::CODE | Type::BOOL}},
	{"INTEGER.=", {equal<int>, Type::INT | Type::BOOL}},
	{"FLOAT.=", {equal<double>, Type::FLOAT | Type::BOOL}},
	{"BOOLEAN.=", {equal<bool>, Type::BOOL}},
	{"EXEC.=", {exec_equal, Type::EXEC | Type::BOOL}},
	{"CODE.POP", {protected_pop<Code_ptr>, Type::CODE}},
	{"INTEGER.POP", {protected_pop<int>, Type::INT}},
	{"FLOAT.POP", {protected_pop<double>, Type::FLOAT}},
	{"BOOLEAN.POP", {protected_pop<bool>, Type::BOOL}},
	{"EXEC.POP", {protected_exec_pop, Type::EXEC}},
	{"CODE.DUP", {dup<Code_ptr>, Type::CODE}},
	{"INTEGER.DUP", {dup<int>, Type::INT}},
	{"FLOAT.DUP", {dup<double>, Type::FLOAT}},
	{"BOOLEAN.DUP", {dup<bool>, Type::BOOL}},
	{"EXEC.DUP", {exec_dup, Type::EXEC}},
	{"CODE.FLUSH", {flush<Code_ptr>, Type::CODE}},
	{"INTEGER.FLUSH", {flush<int>, Type::INT}},
	{"FLOAT.FLUSH", {flush<double>, Type::FLOAT}},
	{"BOOLEAN.FLUSH", {flush<bool>, Type::BOOL}},
	{"EXEC.FLUSH", {exec_flush, Type::EXEC}},
	{"CODE.ROT", {rot<Code_ptr>, Type::CODE}},
	{"INTEGER.ROT", {rot<int>, Type::INT}},
	{"FLOAT.ROT", {rot<double>, Type::FLOAT}},
	{"BOOLEAN.ROT", {rot<bool>, Type::BOOL}},
	{"EXEC.ROT", {exec_rot, Type::EXEC}},
	{"CODE.SHOVE", {shove<Code_ptr>, Type::CODE | Type::INT}},
	{"INTEGER.SHOVE", {shove<int>, Type::INT}},
	{"FLOAT.SHOVE", {shove<double>, Type::FLOAT | Type::INT}},
	{"BOOLEAN.SHOVE", {shove<bool>, Type::BOOL | Type::INT}},
	{"EXEC.SHOVE", {exec_shove, Type::EXEC | Type::INT}},
	{"CODE.STACKDEPTH", {stackdepth<Code_ptr>, Type::CODE | Type::INT}},
	{"INTEGER.STACKDEPTH", {stackdepth<int>, Type::INT}},
	{"FLOAT.STACKDEPTH", {stackdepth<double>, Type::FLOAT | Type::INT}},
	{"BOOLEAN.STACKDEPTH", {stackdepth<bool>, Type::BOOL | Type::INT}},
	{"EXEC.STACKDEPTH", {exec_stackdepth, Type::EXEC | Type::INT}},
	{"CODE.SWAP", {swap<Code_ptr>, Type::CODE}},
	{"INTEGER.SWAP", {swap<int>, Type::INT}},
	{"FLOAT.SWAP", {swap<double>, Type::FLOAT}},
	{"BOOLEAN.SWAP", {swap<bool>, Type::BOOL}},
	{"EXEC.SWAP", {exec_swap, Type::EXEC}},
	{"CODE.YANK", {yank<Code_ptr>, Type::CODE | Type::INT}},
	{"INTEGER.YANK", {yank<int>, Type::INT}},
	{"FLOAT.YANK", {yank<double>, Type::FLOAT | Type::INT}},
	{"BOOLEAN.YANK", {yank<bool>, Type::BOOL | Type::INT}},
	{"EXEC.YANK", {exec_yank, Type::EXEC | Type::INT}},
	{"CODE.YANKDUP", {yankdup<Code_ptr>, Type::CODE | Type::INT}},
	{"INTEGER.YANKDUP", {yankdup<int>, Type::INT}},
	{"FLOAT.YANKDUP", {yankdup<double>, Type::FLOAT | Type::INT}},
	{"BOOLEAN.YANKDUP", {yankdup<bool>, Type::BOOL | Type::INT}},
	{"EXEC.YANKDUP", {exec_yankdup, Type::EXEC | Type::INT}},
};

void load_instruction(std::vector<Instruction_ptr>& instruction_set, std::string name) {
	auto insn = core_instructions.at(name);
	instruction_set.push_back(std::make_shared<Instruction>(std::get<0>(insn), name));
}

} // namespace detail

void register_core(std::vector<Instruction_ptr>& instruction_set) {
	for (const auto& el : detail::core_instructions) {
		detail::load_instruction(instruction_set, el.first);
	}
}

void register_core_by_stack(std::vector<Instruction_ptr>& instruction_set, const Types& types) {
	for (const auto& el : detail::core_instructions) {
		if ((std::get<Types>(el.second) | types) == types) {
			detail::load_instruction(instruction_set, el.first);
		}
	}
}

// Parentheses required
//	// exec
//	EXEC.DO*RANGE	1
//	EXEC.DO*COUNT	1
//	EXEC.DO*TIMES	1
//	EXEC.IF			2
//	EXEC.K			2
//	EXEC.S			3
//	EXEC.Y			1
//
//	// code
//	CODE.QUOTE		1
//
//	// common stack ops
//	EXEC.POP		1
//	EXEC.DUP		1
//	EXEC.ROT		3
//	EXEC.SHOVE		1
//	EXEC.SWAP		2

} // namespace cppush
