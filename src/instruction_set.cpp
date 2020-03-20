#include "instruction_set.h"
#include "bool_ops.h"
#include "code_ops.h"
#include "exec_ops.h"
#include "numeric_ops.h"
#include "common_ops.h"

namespace cppush {

void InstructionSet::register_core() {
	// exec
	register(exec_do_range, "EXEC.DO*RANGE", 1);
	register(exec_do_count, "EXEC.DO*COUNT", 1);
	register(exec_do_times, "EXEC.DO*TIMES", 1);
	register(exec_if, "EXEC.IF", 2);
	register(exec_k, "EXEC.K", 2);
	register(exec_s, "EXEC.S", 3);
	register(exec_y, "EXEC.Y", 1);

	// code
	register(code_append, "CODE.APPEND");
	register(code_atom, "CODE.ATOM");
	register(code_car, "CODE.CAR");
	register(code_cdr, "CODE.CDR");
	register(code_cons, "CODE.CONS");
	register(code_container, "CODE.CONTAINER");
	register(code_contains, "CODE.CONTAINS");
	register(code_do, "CODE.DO");
	register(code_do_star, "CODE.DO*STAR");
	register(code_quote, "CODE.QUOTE", 1);
	register(code_do_range, "CODE.DO*RANGE");
	register(code_do_count, "CODE.DO*COUNT");
	register(code_do_times, "CODE.DO*TIMES");
	register(code_extract, "CODE.EXTRACT");
	register(code_from_bool, "CODE.FROMBOOL");
	register(code_from_float, "CODE.FROMFLOAT");
	register(code_from_int, "CODE.FROMINT");
	register(code_if, "CODE.IF");
	register(code_insert, "CODE.INSERT");
	register(code_length, "CODE.LENGTH");
	register(code_list, "CODE.LIST");
	register(code_member, "CODE.MEMBER");
	register(code_noop, "CODE.NOOP");
	register(code_nth, "CODE.NTH");
	register(code_nthcdr, "CODE.NTHCDR");
	register(code_null, "CODE.NULL");
	register(code_position, "CODE.POSITION");
	register(code_size, "CODE.SIZE");
	register(code_subst, "CODE.SUBST");

	// bool
	register(and_, "BOOLEAN.AND");
	register(or_, "BOOLEAN.OR");
	register(not_, "BOOLEAN.NOT");
	register(nand_, "BOOLEAN.NAND");
	register(nor_, "BOOLEAN.NOR");
	register(xor_, "BOOLEAN.XOR");
	register(invert_first_then_and, "BOOLEAN.INVERT_FIRST_THEN_AND");
	register(invert_second_then_and, "BOOLEAN.INVERT_SECOND_THEN_AND");
	register(bool_from_int, "BOOLEAN.FROMINT");
	register(bool_from_float, "BOOLEAN.FROMFLOAT");

	// int
	register(add, "INTEGER.+");
	register(sub, "INTEGER.-");
	register(mul, "INTEGER.*");
	register(div, "INTEGER./");
	register(int_mod, "INTEGER.%");
	register(less_than, "INTEGER.<");
	register(greater_than, "INTEGER.>");
	register(int_from_bool, "INTEGER.FROMBOOLEAN");
	register(int_from_float, "INTEGER.FROMFLOAT");
	register(max, "INTEGER.MAX");
	register(min, "INTEGER.MIN");

	// float
	register(add<double>, "FLOAT.+");
	register(sub<double>, "FLOAT.-");
	register(mul<double>, "FLOAT.*");
	register(div<double>, "FLOAT./");
	register(float_mod, "FLOAT.%");
	register(less_than<double>, "FLOAT.<");
	register(greater_than<double>, "FLOAT.>");
	register(float_from_bool, "FLOAT.FROMBOOLEAN");
	register(float_from_int, "FLOAT.FROMINTEGER");
	register(max<double>, "FLOAT.MAX");
	register(min<double>, "FLOAT.MIN");
	register(trig_cos, "FLOAT.COS");
	register(trig_sin, "FLOAT.SIN");
	register(trig_tan, "FLOAT.TAN");

	// common
	register(equal<std::shared_ptr<Code>>, "CODE.=");
	register(equal<int>, "INTEGER.=");
	register(equal<double>, "FLOAT.=");
	register(equal<bool>, "BOOLEAN.=");
	register(exec_equal, "EXEC.=");
	register(protected_pop<std::shared_ptr<Code>>, "CODE.POP");
	register(protected_pop<int>, "INTEGER.POP");
	register(protected_pop<double>, "FLOAT.POP");
	register(protected_pop<bool>, "BOOLEAN.POP");
	register(protected_exec_pop, "EXEC.POP", 1);
	register(dup<std::shared_ptr<Code>>, "CODE.DUP");
	register(dup<int>, "INTEGER.DUP");
	register(dup<double>, "FLOAT.DUP");
	register(dup<bool>, "BOOLEAN.DUP");
	register(exec_dup, "EXEC.DUP", 1);
	register(flush<std::shared_ptr<Code>>, "CODE.FLUSH");
	register(flush<int>, "INTEGER.FLUSH");
	register(flush<double>, "FLOAT.FLUSH");
	register(flush<bool>, "BOOLEAN.FLUSH");
	register(exec_flush, "EXEC.FLUSH");
	register(rot<std::shared_ptr<Code>>, "CODE.ROT");
	register(rot<int>, "INTEGER.ROT");
	register(rot<double>, "FLOAT.ROT");
	register(rot<bool>, "BOOLEAN.ROT");
	register(exec_rot, "EXEC.ROT", 3);
	register(shove<std::shared_ptr<Code>>, "CODE.SHOVE");
	register(shove<int>, "INTEGER.SHOVE");
	register(shove<double>, "FLOAT.SHOVE");
	register(shove<bool>, "BOOLEAN.SHOVE");
	register(exec_shove, "EXEC.SHOVE", 1);
	register(stackdepth<std::shared_ptr<Code>>, "CODE.STACKDEPTH");
	register(stackdepth<int>, "INTEGER.STACKDEPTH");
	register(stackdepth<double>, "FLOAT.STACKDEPTH");
	register(stackdepth<bool>, "BOOLEAN.STACKDEPTH");
	register(exec_stackdepth, "EXEC.STACKDEPTH");
	register(swap<std::shared_ptr<Code>>, "CODE.SWAP");
	register(swap<int>, "INTEGER.SWAP");
	register(swap<double>, "FLOAT.SWAP");
	register(swap<bool>, "BOOLEAN.SWAP");
	register(exec_swap, "EXEC.SWAP", 2);
	register(yank<std::shared_ptr<Code>>, "CODE.YANK");
	register(yank<int>, "INTEGER.YANK");
	register(yank<double>, "FLOAT.YANK");
	register(yank<bool>, "BOOLEAN.YANK");
	register(exec_yank, "EXEC.YANK");
	register(yankdup<std::shared_ptr<Code>>, "CODE.YANKDUP");
	register(yankdup<int>, "INTEGER.YANKDUP");
	register(yankdup<double>, "FLOAT.YANKDUP");
	register(yankdup<bool>, "BOOLEAN.YANKDUP");
	register(exec_yankdup, "EXEC.YANKDUP");
}

} // cppush
