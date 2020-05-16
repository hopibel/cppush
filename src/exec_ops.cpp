#include "code.h"
#include "common_ops.h"
#include "env.h"
#include "exec_ops.h"
#include "instruction.h"
#include "instruction_set.h"
#include "literal.h"

#include <memory>
#include <vector>

namespace cppush {

unsigned exec_do_range(Env& env) {
	auto exec_stack_size = env.get_stack<Exec>().size();
	auto int_stack_size = env.get_stack<int>().size();

	// TODO: namespace global
	static const auto do_range_insn = Instruction(exec_do_range, "exec_do*range");

	if (exec_stack_size > 0 && int_stack_size >= 2) {
		int dest = env.pop<int>();
		int index = env.get_stack<int>().back();
		auto code = env.pop<Exec>();

		if (index != dest) {
			index += index > dest ? -1 : 1;

			std::vector<const Code*> rcall{
				env.guard(std::make_unique<Literal<int>>(index)),
				env.guard(std::make_unique<Literal<int>>(dest)),
				&do_range_insn, code
			};

			// TODO(hopibel): push guard unique_ptr to avoid leak
			env.push<Exec>(env.guard(std::make_unique<CodeList>(rcall)));
		}
		env.push<Exec>(code);
	}

	return 1;
}

unsigned exec_do_count(Env& env) {
	auto exec_stack_size = env.get_stack<Exec>().size();
	auto int_stack_size = env.get_stack<int>().size();

	// TODO: namespace global
	static const auto do_range_insn = Instruction(exec_do_range, "exec_do*range");

	if (exec_stack_size > 0 && int_stack_size > 0 && env.get_stack<int>().back() > 0) {
		int count = env.pop<int>();
		auto code = env.pop<Exec>();

		std::vector<const Code*> rcall{
			env.guard(std::make_unique<Literal<int>>(0)), // TODO: namespace global
			env.guard(std::make_unique<Literal<int>>(count - 1)),
			&do_range_insn, code
		};

		env.push<Exec>(env.guard(std::make_unique<CodeList>(rcall)));
	}
	return 1;
}

unsigned exec_do_times(Env& env) {
	auto exec_stack_size = env.get_stack<Exec>().size();
	auto int_stack_size = env.get_stack<int>().size();

	// TODO: namespace global
	static const auto do_range_insn = Instruction(exec_do_range, "exec_do*range");
	static const auto int_pop = Instruction(protected_pop<int>, "integer_pop");

	if (exec_stack_size > 0 && int_stack_size > 0 && env.get_stack<int>().back() > 0) {
		int times = env.pop<int>();
		auto code = env.pop<Exec>();

		auto pop_code = env.guard(std::make_unique<CodeList>(
			std::vector<const Code*>{ &int_pop, code }
		));

		std::vector<const Code*> rcall{
			env.guard(std::make_unique<Literal<int>>(0)), // TODO: namespace global
			env.guard(std::make_unique<Literal<int>>(times - 1)),
			&do_range_insn, pop_code
		};

		// TODO(hopibel): push guard unique_ptr to avoid leak
		env.push<Exec>(env.guard(std::make_unique<CodeList>(rcall)));
	}
	return 1;
}

unsigned exec_if(Env& env) {
	if (env.get_stack<Exec>().size() >= 2 && env.get_stack<bool>().size() > 0) {
		auto first = env.pop<Exec>();
		auto second = env.pop<Exec>();
		if (env.pop<bool>()) {
			env.push<Exec>(first);
		} else {
			env.push<Exec>(second);
		}
	}
	return 1;
}

unsigned exec_k(Env& env) {
	if (env.get_stack<Exec>().size() >= 2) {
		auto first = env.pop<Exec>();
		env.pop<Exec>();
		env.push<Exec>(first);
	}
	return 1;
}

unsigned exec_s(Env& env) {
	if (env.get_stack<Exec>().size() >= 3) {
		auto a = env.pop<Exec>();
		auto b = env.pop<Exec>();
		auto c = env.pop<Exec>();

		env.push<Exec>(env.guard(std::make_unique<CodeList>(
			std::vector<const Code*>{ b, c }
		)));
		env.push<Exec>(c);
		env.push<Exec>(a);
	}
	return 1;
}

unsigned exec_y(Env& env) {
	if (env.get_stack<Exec>().size() > 0) {
		auto first = env.pop<Exec>();
		static const auto y_insn = Instruction(exec_y, "exec_y");

		env.push<Exec>(env.guard(std::make_unique<CodeList>(
			std::vector<const Code*>{ &y_insn, first }
		)));
		env.push<Exec>(first);
	}
	return 1;
}

} // namespace cppush

