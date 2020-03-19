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
	

	// float
	

	// common
	
}

} // cppush
