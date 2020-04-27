#ifndef BOOL_OPS_H
#define BOOL_OPS_H

#include "env.h"

namespace cppush {

unsigned bool_and(Env& env);
unsigned bool_or(Env& env);
unsigned bool_not(Env& env);
unsigned bool_nand(Env& env);
unsigned bool_nor(Env& env);
unsigned bool_xor(Env& env);
unsigned bool_invert_first_then_and(Env& env);
unsigned bool_invert_second_then_and(Env& env);

} // namespace cppush

#endif // BOOL_OPS_H
