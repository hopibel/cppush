#ifndef CODE_OPS_H
#define CODE_OPS_H

#include "code.h"
#include "common_ops.h"
#include "env.h"
#include "instruction.h"
#include "literal.h"

#include <algorithm>
#include <cmath>
#include <memory>

namespace cppush {

unsigned code_append(Env& env);
unsigned code_atom(Env& env);
unsigned code_car(Env& env); // push first element of top item
unsigned code_cdr(Env& env); // push rest of top item (coerced to list)
unsigned code_cons(Env& env);
unsigned code_container(Env& env);
unsigned code_contains(Env& env);
unsigned code_do(Env& env);
unsigned code_do_star(Env& env);
unsigned code_quote(Env& env);
unsigned code_do_range(Env& env);
unsigned code_do_count(Env& env);
unsigned code_do_times(Env& env);
unsigned code_extract(Env& env);
unsigned code_from_bool(Env& env);
unsigned code_from_float(Env& env);
unsigned code_from_int(Env& env);
unsigned code_if(Env& env);
unsigned code_insert(Env& env);
unsigned code_length(Env& env);
unsigned code_list(Env& env);
unsigned code_member(Env& env);
unsigned code_noop(Env&);
unsigned code_nth(Env& env);
unsigned code_nthcdr(Env& env);
unsigned code_null(Env& env);
unsigned code_position(Env& env);
unsigned code_size(Env& env);
unsigned code_subst(Env& env);

} // namespace cppush

#endif // CODE_OPS_H
