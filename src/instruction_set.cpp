#include "instruction_set.h"
#include "bool_ops.h"
#include "code_ops.h"
#include "exec_ops.h"
#include "numeric_ops.h"
#include "common_ops.h"

namespace cppush {

// TODO(hopibel): should be InstructionManager that can load core insns and return subsets as vectors
void InstructionSet::register_core() {
	// exec
	register(exec_do_range, "EXEC.DO*RANGE", EXEC | INT, 1);
	register(exec_do_count, "EXEC.DO*COUNT", EXEC | INT, 1);
	register(exec_do_times, "EXEC.DO*TIMES", EXEC | INT, 1);
	register(exec_if, "EXEC.IF", EXEC | BOOL, 2);
	register(exec_k, "EXEC.K", EXEC, 2);
	register(exec_s, "EXEC.S", EXEC, 3);
	register(exec_y, "EXEC.Y", EXEC, 1);

	// code
	register(code_append, "CODE.APPEND", CODE);
	register(code_atom, "CODE.ATOM", CODE);
	register(code_car, "CODE.CAR", CODE);
	register(code_cdr, "CODE.CDR", CODE);
	register(code_cons, "CODE.CONS", CODE);
	register(code_container, "CODE.CONTAINER", CODE);
	register(code_contains, "CODE.CONTAINS", CODE | BOOL);
	register(code_do, "CODE.DO", CODE | EXEC);
	register(code_do_star, "CODE.DO*STAR", CODE | EXEC);
	register(code_quote, "CODE.QUOTE", CODE | EXEC, 1);
	register(code_do_range, "CODE.DO*RANGE", CODE | EXEC | INT);
	register(code_do_count, "CODE.DO*COUNT", CODE | EXEC | INT);
	register(code_do_times, "CODE.DO*TIMES", CODE | EXEC | INT);
	register(code_extract, "CODE.EXTRACT", CODE | INT);
	register(code_from_bool, "CODE.FROMBOOL", CODE | BOOL);
	register(code_from_float, "CODE.FROMFLOAT", CODE | FLOAT);
	register(code_from_int, "CODE.FROMINT", CODE | INT);
	register(code_if, "CODE.IF", CODE | BOOL | EXEC);
	register(code_insert, "CODE.INSERT", CODE | INT);
	register(code_length, "CODE.LENGTH", CODE | INT);
	register(code_list, "CODE.LIST", CODE);
	register(code_member, "CODE.MEMBER", CODE | BOOL);
	register(code_noop, "CODE.NOOP", 0);
	register(code_nth, "CODE.NTH", CODE | INT);
	register(code_nthcdr, "CODE.NTHCDR", CODE | INT);
	register(code_null, "CODE.NULL", CODE | BOOL);
	register(code_position, "CODE.POSITION", CODE | INT);
	register(code_size, "CODE.SIZE", CODE | INT);
	register(code_subst, "CODE.SUBST", CODE);

	// bool
	register(and_, "BOOLEAN.AND", BOOL);
	register(or_, "BOOLEAN.OR", BOOL);
	register(not_, "BOOLEAN.NOT", BOOL);
	register(nand_, "BOOLEAN.NAND", BOOL);
	register(nor_, "BOOLEAN.NOR", BOOL);
	register(xor_, "BOOLEAN.XOR", BOOL);
	register(invert_first_then_and, "BOOLEAN.INVERT_FIRST_THEN_AND", BOOL);
	register(invert_second_then_and, "BOOLEAN.INVERT_SECOND_THEN_AND", BOOL);
	register(bool_from_int, "BOOLEAN.FROMINT", BOOL | INT);
	register(bool_from_float, "BOOLEAN.FROMFLOAT", BOOL | FLOAT);

	// int
	register(add, "INTEGER.+", INT);
	register(sub, "INTEGER.-", INT);
	register(mul, "INTEGER.*", INT);
	register(div, "INTEGER./", INT);
	register(int_mod, "INTEGER.%", INT);
	register(less_than, "INTEGER.<", INT | BOOL);
	register(greater_than, "INTEGER.>", INT | BOOL);
	register(int_from_bool, "INTEGER.FROMBOOLEAN", INT | BOOL);
	register(int_from_float, "INTEGER.FROMFLOAT", INT | FLOAT);
	register(max, "INTEGER.MAX", INT);
	register(min, "INTEGER.MIN", INT);

	// float
	register(add<double>, "FLOAT.+", FLOAT);
	register(sub<double>, "FLOAT.-", FLOAT);
	register(mul<double>, "FLOAT.*", FLOAT);
	register(div<double>, "FLOAT./", FLOAT);
	register(float_mod, "FLOAT.%", FLOAT);
	register(less_than<double>, "FLOAT.<", FLOAT | BOOL);
	register(greater_than<double>, "FLOAT.>", FLOAT | BOOL);
	register(float_from_bool, "FLOAT.FROMBOOLEAN", FLOAT | BOOL);
	register(float_from_int, "FLOAT.FROMINTEGER", FLOAT | INT);
	register(max<double>, "FLOAT.MAX", FLOAT);
	register(min<double>, "FLOAT.MIN", FLOAT);
	register(trig_cos, "FLOAT.COS", FLOAT);
	register(trig_sin, "FLOAT.SIN", FLOAT);
	register(trig_tan, "FLOAT.TAN", FLOAT);

	// common
	register(equal<std::shared_ptr<Code>>, "CODE.=", CODE | BOOL);
	register(equal<int>, "INTEGER.=", INT | BOOL);
	register(equal<double>, "FLOAT.=", FLOAT | BOOL);
	register(equal<bool>, "BOOLEAN.=", BOOL);
	register(exec_equal, "EXEC.=", EXEC | BOOL);
	register(protected_pop<std::shared_ptr<Code>>, "CODE.POP", CODE);
	register(protected_pop<int>, "INTEGER.POP", INT);
	register(protected_pop<double>, "FLOAT.POP", FLOAT);
	register(protected_pop<bool>, "BOOLEAN.POP", BOOL);
	register(protected_exec_pop, "EXEC.POP", EXEC, 1);
	register(dup<std::shared_ptr<Code>>, "CODE.DUP", CODE);
	register(dup<int>, "INTEGER.DUP", INT);
	register(dup<double>, "FLOAT.DUP", FLOAT);
	register(dup<bool>, "BOOLEAN.DUP", BOOL);
	register(exec_dup, "EXEC.DUP", EXEC, 1);
	register(flush<std::shared_ptr<Code>>, "CODE.FLUSH", CODE);
	register(flush<int>, "INTEGER.FLUSH", INT);
	register(flush<double>, "FLOAT.FLUSH", FLOAT);
	register(flush<bool>, "BOOLEAN.FLUSH". BOOL);
	register(exec_flush, "EXEC.FLUSH", EXEC);
	register(rot<std::shared_ptr<Code>>, "CODE.ROT", CODE);
	register(rot<int>, "INTEGER.ROT", INT);
	register(rot<double>, "FLOAT.ROT", FLOAT);
	register(rot<bool>, "BOOLEAN.ROT", BOOL);
	register(exec_rot, "EXEC.ROT", EXEC, 3);
	register(shove<std::shared_ptr<Code>>, "CODE.SHOVE", CODE | INT);
	register(shove<int>, "INTEGER.SHOVE", INT);
	register(shove<double>, "FLOAT.SHOVE", FLOAT | INT);
	register(shove<bool>, "BOOLEAN.SHOVE", BOOL | INT);
	register(exec_shove, "EXEC.SHOVE", EXEC | INT, 1);
	register(stackdepth<std::shared_ptr<Code>>, "CODE.STACKDEPTH", CODE | INT);
	register(stackdepth<int>, "INTEGER.STACKDEPTH", INT);
	register(stackdepth<double>, "FLOAT.STACKDEPTH", FLOAT | INT);
	register(stackdepth<bool>, "BOOLEAN.STACKDEPTH", BOOL | INT);
	register(exec_stackdepth, "EXEC.STACKDEPTH", EXEC | INT);
	register(swap<std::shared_ptr<Code>>, "CODE.SWAP", CODE);
	register(swap<int>, "INTEGER.SWAP", INT);
	register(swap<double>, "FLOAT.SWAP", FLOAT);
	register(swap<bool>, "BOOLEAN.SWAP", BOOL);
	register(exec_swap, "EXEC.SWAP", EXEC, 2);
	register(yank<std::shared_ptr<Code>>, "CODE.YANK", CODE | INT);
	register(yank<int>, "INTEGER.YANK", INT);
	register(yank<double>, "FLOAT.YANK", FLOAT | INT);
	register(yank<bool>, "BOOLEAN.YANK", BOOL | INT);
	register(exec_yank, "EXEC.YANK", EXEC | INT);
	register(yankdup<std::shared_ptr<Code>>, "CODE.YANKDUP", CODE | INT);
	register(yankdup<int>, "INTEGER.YANKDUP", INT);
	register(yankdup<double>, "FLOAT.YANKDUP", FLOAT | INT);
	register(yankdup<bool>, "BOOLEAN.YANKDUP", BOOL | INT);
	register(exec_yankdup, "EXEC.YANKDUP", EXEC | INT);
}

} // cppush
