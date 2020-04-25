#include "instruction.h"
#include "instruction_manager.h"
#include "bool_ops.h"
#include "code_ops.h"
#include "exec_ops.h"
#include "numeric_ops.h"
#include "common_ops.h"

#include <map>

namespace cppush {

// TODO(hopibel): should be InstructionManager that can load core insns and return subsets as vectors
void InstructionManager::register_core() {
	// exec
	register_op(exec_do_range, "EXEC.DO*RANGE", EXEC | INT, 1);
	register_op(exec_do_count, "EXEC.DO*COUNT", EXEC | INT, 1);
	register_op(exec_do_times, "EXEC.DO*TIMES", EXEC | INT, 1);
	register_op(exec_if, "EXEC.IF", EXEC | BOOL, 2);
	register_op(exec_k, "EXEC.K", EXEC, 2);
	register_op(exec_s, "EXEC.S", EXEC, 3);
	register_op(exec_y, "EXEC.Y", EXEC, 1);

	// code
	register_op(code_append, "CODE.APPEND", CODE);
	register_op(code_atom, "CODE.ATOM", CODE);
	register_op(code_car, "CODE.CAR", CODE);
	register_op(code_cdr, "CODE.CDR", CODE);
	register_op(code_cons, "CODE.CONS", CODE);
	register_op(code_container, "CODE.CONTAINER", CODE);
	register_op(code_contains, "CODE.CONTAINS", CODE | BOOL);
	register_op(code_do, "CODE.DO", CODE | EXEC);
	register_op(code_do_star, "CODE.DO*STAR", CODE | EXEC);
	register_op(code_quote, "CODE.QUOTE", CODE | EXEC, 1);
	register_op(code_do_range, "CODE.DO*RANGE", CODE | EXEC | INT);
	register_op(code_do_count, "CODE.DO*COUNT", CODE | EXEC | INT);
	register_op(code_do_times, "CODE.DO*TIMES", CODE | EXEC | INT);
	register_op(code_extract, "CODE.EXTRACT", CODE | INT);
	register_op(code_from_bool, "CODE.FROMBOOL", CODE | BOOL);
	register_op(code_from_float, "CODE.FROMFLOAT", CODE | FLOAT);
	register_op(code_from_int, "CODE.FROMINT", CODE | INT);
	register_op(code_if, "CODE.IF", CODE | BOOL | EXEC);
	register_op(code_insert, "CODE.INSERT", CODE | INT);
	register_op(code_length, "CODE.LENGTH", CODE | INT);
	register_op(code_list, "CODE.LIST", CODE);
	register_op(code_member, "CODE.MEMBER", CODE | BOOL);
	register_op(code_noop, "CODE.NOOP", 0);
	register_op(code_nth, "CODE.NTH", CODE | INT);
	register_op(code_nthcdr, "CODE.NTHCDR", CODE | INT);
	register_op(code_null, "CODE.NULL", CODE | BOOL);
	register_op(code_position, "CODE.POSITION", CODE | INT);
	register_op(code_size, "CODE.SIZE", CODE | INT);
	register_op(code_subst, "CODE.SUBST", CODE);

	// bool
	register_op(and_, "BOOLEAN.AND", BOOL);
	register_op(or_, "BOOLEAN.OR", BOOL);
	register_op(not_, "BOOLEAN.NOT", BOOL);
	register_op(nand_, "BOOLEAN.NAND", BOOL);
	register_op(nor_, "BOOLEAN.NOR", BOOL);
	register_op(xor_, "BOOLEAN.XOR", BOOL);
	register_op(invert_first_then_and, "BOOLEAN.INVERT_FIRST_THEN_AND", BOOL);
	register_op(invert_second_then_and, "BOOLEAN.INVERT_SECOND_THEN_AND", BOOL);
	register_op(bool_from_int, "BOOLEAN.FROMINT", BOOL | INT);
	register_op(bool_from_float, "BOOLEAN.FROMFLOAT", BOOL | FLOAT);

	// int
	register_op(add<int>, "INTEGER.+", INT);
	register_op(sub<int>, "INTEGER.-", INT);
	register_op(mul<int>, "INTEGER.*", INT);
	register_op(div<int>, "INTEGER./", INT);
	register_op(int_mod, "INTEGER.%", INT);
	register_op(less_than<int>, "INTEGER.<", INT | BOOL);
	register_op(greater_than<int>, "INTEGER.>", INT | BOOL);
	register_op(int_from_bool, "INTEGER.FROMBOOLEAN", INT | BOOL);
	register_op(int_from_float, "INTEGER.FROMFLOAT", INT | FLOAT);
	register_op(max<int>, "INTEGER.MAX", INT);
	register_op(min<int>, "INTEGER.MIN", INT);

	// float
	register_op(add<double>, "FLOAT.+", FLOAT);
	register_op(sub<double>, "FLOAT.-", FLOAT);
	register_op(mul<double>, "FLOAT.*", FLOAT);
	register_op(div<double>, "FLOAT./", FLOAT);
	register_op(float_mod, "FLOAT.%", FLOAT);
	register_op(less_than<double>, "FLOAT.<", FLOAT | BOOL);
	register_op(greater_than<double>, "FLOAT.>", FLOAT | BOOL);
	register_op(float_from_bool, "FLOAT.FROMBOOLEAN", FLOAT | BOOL);
	register_op(float_from_int, "FLOAT.FROMINTEGER", FLOAT | INT);
	register_op(max<double>, "FLOAT.MAX", FLOAT);
	register_op(min<double>, "FLOAT.MIN", FLOAT);
	register_op(trig_cos, "FLOAT.COS", FLOAT);
	register_op(trig_sin, "FLOAT.SIN", FLOAT);
	register_op(trig_tan, "FLOAT.TAN", FLOAT);

	// common stack ops
	register_op(equal<std::shared_ptr<Code>>, "CODE.=", CODE | BOOL);
	register_op(equal<int>, "INTEGER.=", INT | BOOL);
	register_op(equal<double>, "FLOAT.=", FLOAT | BOOL);
	register_op(equal<bool>, "BOOLEAN.=", BOOL);
	register_op(exec_equal, "EXEC.=", EXEC | BOOL);
	register_op(protected_pop<std::shared_ptr<Code>>, "CODE.POP", CODE);
	register_op(protected_pop<int>, "INTEGER.POP", INT);
	register_op(protected_pop<double>, "FLOAT.POP", FLOAT);
	register_op(protected_pop<bool>, "BOOLEAN.POP", BOOL);
	register_op(protected_exec_pop, "EXEC.POP", EXEC, 1);
	register_op(dup<std::shared_ptr<Code>>, "CODE.DUP", CODE);
	register_op(dup<int>, "INTEGER.DUP", INT);
	register_op(dup<double>, "FLOAT.DUP", FLOAT);
	register_op(dup<bool>, "BOOLEAN.DUP", BOOL);
	register_op(exec_dup, "EXEC.DUP", EXEC, 1);
	register_op(flush<std::shared_ptr<Code>>, "CODE.FLUSH", CODE);
	register_op(flush<int>, "INTEGER.FLUSH", INT);
	register_op(flush<double>, "FLOAT.FLUSH", FLOAT);
	register_op(flush<bool>, "BOOLEAN.FLUSH", BOOL);
	register_op(exec_flush, "EXEC.FLUSH", EXEC);
	register_op(rot<std::shared_ptr<Code>>, "CODE.ROT", CODE);
	register_op(rot<int>, "INTEGER.ROT", INT);
	register_op(rot<double>, "FLOAT.ROT", FLOAT);
	register_op(rot<bool>, "BOOLEAN.ROT", BOOL);
	register_op(exec_rot, "EXEC.ROT", EXEC, 3);
	register_op(shove<std::shared_ptr<Code>>, "CODE.SHOVE", CODE | INT);
	register_op(shove<int>, "INTEGER.SHOVE", INT);
	register_op(shove<double>, "FLOAT.SHOVE", FLOAT | INT);
	register_op(shove<bool>, "BOOLEAN.SHOVE", BOOL | INT);
	register_op(exec_shove, "EXEC.SHOVE", EXEC | INT, 1);
	register_op(stackdepth<std::shared_ptr<Code>>, "CODE.STACKDEPTH", CODE | INT);
	register_op(stackdepth<int>, "INTEGER.STACKDEPTH", INT);
	register_op(stackdepth<double>, "FLOAT.STACKDEPTH", FLOAT | INT);
	register_op(stackdepth<bool>, "BOOLEAN.STACKDEPTH", BOOL | INT);
	register_op(exec_stackdepth, "EXEC.STACKDEPTH", EXEC | INT);
	register_op(swap<std::shared_ptr<Code>>, "CODE.SWAP", CODE);
	register_op(swap<int>, "INTEGER.SWAP", INT);
	register_op(swap<double>, "FLOAT.SWAP", FLOAT);
	register_op(swap<bool>, "BOOLEAN.SWAP", BOOL);
	register_op(exec_swap, "EXEC.SWAP", EXEC, 2);
	register_op(yank<std::shared_ptr<Code>>, "CODE.YANK", CODE | INT);
	register_op(yank<int>, "INTEGER.YANK", INT);
	register_op(yank<double>, "FLOAT.YANK", FLOAT | INT);
	register_op(yank<bool>, "BOOLEAN.YANK", BOOL | INT);
	register_op(exec_yank, "EXEC.YANK", EXEC | INT);
	register_op(yankdup<std::shared_ptr<Code>>, "CODE.YANKDUP", CODE | INT);
	register_op(yankdup<int>, "INTEGER.YANKDUP", INT);
	register_op(yankdup<double>, "FLOAT.YANKDUP", FLOAT | INT);
	register_op(yankdup<bool>, "BOOLEAN.YANKDUP", BOOL | INT);
	register_op(exec_yankdup, "EXEC.YANKDUP", EXEC | INT);
}

void InstructionManager::register_op(unsigned (*op)(Env&), std::string name, int typemask, unsigned parens) {
	insns_[name] = std::make_shared<Instruction>(op, name, typemask, parens);
}

std::vector<std::shared_ptr<Instruction>> InstructionManager::get_by_stack(int bitmask, int exclude_mask) {
	std::vector<std::shared_ptr<Instruction>> ops;
	for (auto const& el : insns_) {
		if ((el.second->types & bitmask) > 0 && (el.second->types & exclude_mask) == 0) {
			ops.push_back(el.second);
		}
	}
	return ops;
}

} // namespace cppush
