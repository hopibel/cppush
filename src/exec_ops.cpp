#include "code.h"
#include "common_ops.h"
#include "env.h"
#include "exec_ops.h"
#include "instruction.h"
#include "literal.h"

#include <memory>
#include <vector>

namespace cppush {

unsigned exec_do_range(Env& env) {
	auto exec_stack_size = env.get_exec_stack().size();
	auto int_stack_size = env.get_stack<int>().size();

	if (exec_stack_size > 0 && int_stack_size >= 2) {
		int dest = env.pop<int>();
		int index = env.get_stack<int>().back();
		auto code = env.pop_exec();

		if (index != dest) {
			index += index > dest ? -1 : 1;

			static auto do_range_insn = std::make_shared<Instruction>(
					exec_do_range, "EXEC.DO*RANGE");

			std::vector<std::shared_ptr<Code>> rcall{
					std::make_shared<Literal<int>>(index),
					std::make_shared<Literal<int>>(dest),
					do_range_insn, code
			};

			env.get_exec_stack().push_back(std::make_shared<CodeList>(rcall));
		}
		env.get_exec_stack().push_back(code);
	}

	return 1;
}

unsigned exec_do_count(Env& env) {
	auto exec_stack_size = env.get_exec_stack().size();
	auto int_stack_size = env.get_stack<int>().size();

	if (exec_stack_size > 0 && int_stack_size > 0 && env.get_stack<int>().back() > 0) {
		int count = env.pop<int>();
		auto code = env.pop_exec();

		static auto do_range_insn = std::make_shared<Instruction>(
				exec_do_range, "EXEC.DO*RANGE");

		std::vector<std::shared_ptr<Code>> rcall{
				std::make_shared<Literal<int>>(0),
				std::make_shared<Literal<int>>(count - 1),
				do_range_insn, code
		};

		env.get_exec_stack().push_back(std::make_shared<CodeList>(rcall));
	}
	return 1;
}

unsigned exec_do_times(Env& env) {
	auto exec_stack_size = env.get_exec_stack().size();
	auto int_stack_size = env.get_stack<int>().size();

	if (exec_stack_size > 0 && int_stack_size > 0 && env.get_stack<int>().back() > 0) {
		int times = env.pop<int>();
		auto code = env.pop_exec();

		static auto do_range_insn = std::make_shared<Instruction>(
				exec_do_range, "EXEC.DO*RANGE");
		static auto int_pop = std::make_shared<Instruction>(
				protected_pop<int>, "INTEGER.POP");

		std::vector<std::shared_ptr<Code>> pop_code{int_pop, code};

		std::vector<std::shared_ptr<Code>> rcall{
				std::make_shared<Literal<int>>(0),
				std::make_shared<Literal<int>>(times - 1),
				do_range_insn, std::make_shared<CodeList>(pop_code)
		};

		env.get_exec_stack().push_back(std::make_shared<CodeList>(rcall));
	}
	return 1;
}

unsigned exec_if(Env& env) {
	if (env.get_exec_stack().size() >= 2 && env.get_stack<bool>().size() > 0) {
		auto first = env.pop_exec();
		auto second = env.pop_exec();
		if (env.pop<bool>()) {
			env.get_exec_stack().push_back(first);
		} else {
			env.get_exec_stack().push_back(second);
		}
	}
	return 1;
}

unsigned exec_k(Env& env) {
	if (env.get_exec_stack().size() >= 2) {
		auto first = env.pop_exec();
		auto second = env.pop_exec();
		env.get_exec_stack().push_back(first);
	}
	return 1;
}

unsigned exec_s(Env& env) {
	auto& stack = env.get_exec_stack();
	if (stack.size() >= 3) {
		auto a = env.pop_exec();
		auto b = env.pop_exec();
		auto c = env.pop_exec();

		std::vector<std::shared_ptr<Code>> bc{b, c};

		stack.push_back(std::make_shared<CodeList>(bc));
		stack.push_back(c);
		stack.push_back(a);
	}
	return 1;
}

unsigned exec_y(Env& env) {
	if (env.get_exec_stack().size() > 0) {
		auto first = env.pop_exec();
		static auto y_insn = std::make_shared<Instruction>(
				exec_y, "EXEC.Y");

		std::vector<std::shared_ptr<Code>> rcall{y_insn, first};
		env.get_exec_stack().push_back(std::make_shared<CodeList>(rcall));
		env.get_exec_stack().push_back(first);
	}
	return 1;
}

} // namespace cppush

