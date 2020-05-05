#include "instruction_set.h"

#include "code.h"
#include "instruction.h"
#include "input_instruction.h"
#include "bool_ops.h"
#include "code_ops.h"
#include "common_ops.h"
#include "exec_ops.h"
#include "numeric_ops.h"
#include "types.h"

#include <utility>
#include <string>

namespace cppush {

namespace detail {

const std::map<std::string, std::pair<unsigned (*)(Env&), Types>> core_instructions{
	// exec
	{"exec_do*range", {exec_do_range, Type::Exec | Type::Int}},
	{"exec_do*count", {exec_do_count, Type::Exec | Type::Int}},
	{"exec_do*times", {exec_do_times, Type::Exec | Type::Int}},
	{"exec_if", {exec_if, Type::Exec | Type::Bool}},
	{"exec_k", {exec_k, Type::Exec}},
	{"exec_s", {exec_s, Type::Exec}},
	{"exec_y", {exec_y, Type::Exec}},

	// code
	{"code_append", {code_append, Type::Code}},
	{"code_atom", {code_atom, Type::Code}},
	{"code_car", {code_car, Type::Code}},
	{"code_cdr", {code_cdr, Type::Code}},
	{"code_cons", {code_cons, Type::Code}},
	{"code_container", {code_container, Type::Code}},
	{"code_contains", {code_contains, Type::Code | Type::Bool}},
	{"code_do", {code_do, Type::Code | Type::Exec}},
	{"code_do*star", {code_do_star, Type::Code | Type::Exec}},
	{"code_quote", {code_quote, Type::Code | Type::Exec}},
	{"code_do*range", {code_do_range, Type::Code | Type::Exec | Type::Int}},
	{"code_do*count", {code_do_count, Type::Code | Type::Exec | Type::Int}},
	{"code_do*times", {code_do_times, Type::Code | Type::Exec | Type::Int}},
	{"code_extract", {code_extract, Type::Code | Type::Int}},
	{"code_frombool", {code_from_bool, Type::Code | Type::Bool}},
	{"code_fromfloat", {code_from_float, Type::Code | Type::Float}},
	{"code_fromint", {code_from_int, Type::Code | Type::Int}},
	{"code_if", {code_if, Type::Code | Type::Bool | Type::Exec}},
	{"code_insert", {code_insert, Type::Code | Type::Int}},
	{"code_length", {code_length, Type::Code | Type::Int}},
	{"code_list", {code_list, Type::Code}},
	{"code_member", {code_member, Type::Code | Type::Bool}},
	{"code_noop", {code_noop, Type(0)}},
	{"code_nth", {code_nth, Type::Code | Type::Int}},
	{"code_nthcdr", {code_nthcdr, Type::Code | Type::Int}},
	{"code_null", {code_null, Type::Code | Type::Bool}},
	{"code_position", {code_position, Type::Code | Type::Int}},
	{"code_size", {code_size, Type::Code | Type::Int}},
	{"code_subst", {code_subst, Type::Code}},

	// bool
	{"boolean_and", {bool_and, Type::Bool}},
	{"boolean_or", {bool_or, Type::Bool}},
	{"boolean_not", {bool_not, Type::Bool}},
	{"boolean_nand", {bool_nand, Type::Bool}},
	{"boolean_nor", {bool_nor, Type::Bool}},
	{"boolean_xor", {bool_xor, Type::Bool}},
	{"boolean_invert_first_then_and", {bool_invert_first_then_and, Type::Bool}},
	{"boolean_invert_second_then_and", {bool_invert_second_then_and, Type::Bool}},
	{"boolean_fromint", {bool_from_int, Type::Bool | Type::Int}},
	{"boolean_fromfloat", {bool_from_float, Type::Bool | Type::Float}},

	// int
	{"integer_add", {add<int>, Type::Int}},
	{"integer_sub", {sub<int>, Type::Int}},
	{"integer_mult", {mul<int>, Type::Int}},
	{"integer_div", {div<int>, Type::Int}},
	{"integer_mod", {int_mod, Type::Int}},
	{"integer_lt", {lt<int>, Type::Int | Type::Bool}},
	{"integer_gt", {gt<int>, Type::Int | Type::Bool}},
	{"integer_fromboolean", {int_from_bool, Type::Int | Type::Bool}},
	{"integer_fromfloat", {int_from_float, Type::Int | Type::Float}},
	{"integer_max", {max<int>, Type::Int}},
	{"integer_min", {min<int>, Type::Int}},

	// float
	{"float_add", {add<double>, Type::Float}},
	{"float_sub", {sub<double>, Type::Float}},
	{"float_mult", {mul<double>, Type::Float}},
	{"float_div", {div<double>, Type::Float}},
	{"float_mod", {float_mod, Type::Float}},
	{"float_lt", {lt<double>, Type::Float | Type::Bool}},
	{"float_gt", {gt<double>, Type::Float | Type::Bool}},
	{"float_fromboolean", {float_from_bool, Type::Float | Type::Bool}},
	{"float_frominteger", {float_from_int, Type::Float | Type::Int}},
	{"float_max", {max<double>, Type::Float}},
	{"float_min", {min<double>, Type::Float}},
	{"float_cos", {trig_cos, Type::Float}},
	{"float_sin", {trig_sin, Type::Float}},
	{"float_tan", {trig_tan, Type::Float}},

	// common stack ops
	{"code_eq", {equal<Code>, Type::Code | Type::Bool}},
	{"integer_eq", {equal<int>, Type::Int | Type::Bool}},
	{"float_eq", {equal<double>, Type::Float | Type::Bool}},
	{"boolean_eq", {equal<bool>, Type::Bool}},
	{"exec_eq", {equal<Exec>, Type::Exec | Type::Bool}},
	{"code_pop", {protected_pop<Code>, Type::Code}},
	{"integer_pop", {protected_pop<int>, Type::Int}},
	{"float_pop", {protected_pop<double>, Type::Float}},
	{"boolean_pop", {protected_pop<bool>, Type::Bool}},
	{"exec_pop", {protected_pop<Exec>, Type::Exec}},
	{"code_dup", {dup<Code>, Type::Code}},
	{"integer_dup", {dup<int>, Type::Int}},
	{"float_dup", {dup<double>, Type::Float}},
	{"boolean_dup", {dup<bool>, Type::Bool}},
	{"exec_dup", {dup<Exec>, Type::Exec}},
	{"code_flush", {flush<Code>, Type::Code}},
	{"integer_flush", {flush<int>, Type::Int}},
	{"float_flush", {flush<double>, Type::Float}},
	{"boolean_flush", {flush<bool>, Type::Bool}},
	{"exec_flush", {flush<Exec>, Type::Exec}},
	{"code_rot", {rot<Code>, Type::Code}},
	{"integer_rot", {rot<int>, Type::Int}},
	{"float_rot", {rot<double>, Type::Float}},
	{"boolean_rot", {rot<bool>, Type::Bool}},
	{"exec_rot", {rot<Exec>, Type::Exec}},
	{"code_shove", {shove<Code>, Type::Code | Type::Int}},
	{"integer_shove", {shove<int>, Type::Int}},
	{"float_shove", {shove<double>, Type::Float | Type::Int}},
	{"boolean_shove", {shove<bool>, Type::Bool | Type::Int}},
	{"exec_shove", {shove<Exec>, Type::Exec | Type::Int}},
	{"code_stackdepth", {stackdepth<Code>, Type::Code | Type::Int}},
	{"integer_stackdepth", {stackdepth<int>, Type::Int}},
	{"float_stackdepth", {stackdepth<double>, Type::Float | Type::Int}},
	{"boolean_stackdepth", {stackdepth<bool>, Type::Bool | Type::Int}},
	{"exec_stackdepth", {stackdepth<Exec>, Type::Exec | Type::Int}},
	{"code_swap", {swap<Code>, Type::Code}},
	{"integer_swap", {swap<int>, Type::Int}},
	{"float_swap", {swap<double>, Type::Float}},
	{"boolean_swap", {swap<bool>, Type::Bool}},
	{"exec_swap", {swap<Exec>, Type::Exec}},
	{"code_yank", {yank<Code>, Type::Code | Type::Int}},
	{"integer_yank", {yank<int>, Type::Int}},
	{"float_yank", {yank<double>, Type::Float | Type::Int}},
	{"boolean_yank", {yank<bool>, Type::Bool | Type::Int}},
	{"exec_yank", {yank<Exec>, Type::Exec | Type::Int}},
	{"code_yankdup", {yankdup<Code>, Type::Code | Type::Int}},
	{"integer_yankdup", {yankdup<int>, Type::Int}},
	{"float_yankdup", {yankdup<double>, Type::Float | Type::Int}},
	{"boolean_yankdup", {yankdup<bool>, Type::Bool | Type::Int}},
	{"exec_yankdup", {yankdup<Exec>, Type::Exec | Type::Int}},
};

void load_instruction(std::vector<Code>& instruction_set, std::string name) {
	auto insn = core_instructions.at(name);
	instruction_set.push_back(std::make_shared<Instruction>(std::get<0>(insn), name));
}

} // namespace detail

void register_core(std::vector<Code>& instruction_set) {
	for (const auto& el : detail::core_instructions) {
		detail::load_instruction(instruction_set, el.first);
	}
}

void register_core_by_stack(std::vector<Code>& instruction_set, const Types& types) {
	for (const auto& el : detail::core_instructions) {
		if ((std::get<Types>(el.second) | types) == types) {
			detail::load_instruction(instruction_set, el.first);
		}
	}
}

void register_n_inputs(std::vector<Code>& instruction_set, int n) {
	for (int i = 0; i < n; ++i) {
		instruction_set.push_back(std::make_shared<InputInstruction>("input_"+i, i));
	}
}

// Parentheses required
//	// exec
//	exec_do*range	1
//	exec_do*count	1
//	exec_do*times	1
//	exec_if			2
//	exec_k			2
//	exec_s			3
//	exec_y			1
//
//	// code
//	code_quote		1
//
//	// common stack ops
//	exec_pop		1
//	exec_dup		1
//	exec_rot		3
//	exec_shove		1
//	exec_swap		2

} // namespace cppush
