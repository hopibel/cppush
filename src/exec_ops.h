#ifndef EXEC_OPS_H
#define EXEC_OPS_H

#include "code.h"
#include "common_ops.h"
#include "env.h"

#include <memory>
#include <vector>

namespace cppush {

unsigned exec_do_range(Env& env);
unsigned exec_do_count(Env& env);
unsigned exec_do_times(Env& env);
unsigned exec_if(Env& env);
unsigned exec_k(Env& env);
unsigned exec_s(Env& env);
unsigned exec_y(Env& env);

} // namespace cppush

#endif // EXEC_OPS_H
