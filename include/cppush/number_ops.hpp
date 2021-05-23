#ifndef NUMBER_OPS_HPP
#define NUMBER_OPS_HPP

namespace cppush {

class State;

unsigned number_add(State&);
unsigned number_sub(State&);
unsigned number_mul(State&);
unsigned number_div(State&);
unsigned number_mod(State&);
unsigned number_max(State&);
unsigned number_min(State&);
unsigned number_cos(State&);
unsigned number_sin(State&);
unsigned number_tan(State&);

} // namespace cppush

#endif // NUMBER_OPS_HPP