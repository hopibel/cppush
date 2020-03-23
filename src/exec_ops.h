#ifndef EXEC_OPS_H
#define EXEC_OPS_H

#include "code.h"
#include "env.h"
#include "instruction.h"

#include <memory>
#include <vector>

namespace cppush {

inline unsigned exec_do_range(Env& env) {
	auto exec_stack_size = get_exec_stack(env).size();
	auto int_stack_size = get_stack<int>(env).size();

	if (exec_stack_size > 0 && int_stack_size >= 2) {
		int dest = pop<int>(env);
		int index = get_stack<int>(env).back();
		auto code = pop_exec(env);

		if (index != dest) {
			index += index > dest ? -1 : 1;

			static auto do_range_insn = std::make_shared<Instruction>(
					exec_do_range,
					"EXEC.DO*RANGE",
					EXEC | INT,
					1);

			std::vector<std::shared_ptr<Code>> rcall{
					std::make_shared<Literal<int>>(index),
					std::make_shared<Literal<int>>(dest),
					do_range_insn, code
			};

			get_exec_stack(env).push_back(std::make_shared<CodeList>(rcall));
		}
		get_exec_stack(env).push_back(code);
	}

	return 1;
}

inline unsigned exec_do_count(Env& env) {
	auto exec_stack_size = get_exec_stack(env).size();
	auto int_stack_size = get_stack<int>(env).size();

	if (exec_stack_size > 0 && int_stack_size > 0) {
		int count = pop<int>(env);
		auto code = pop_exec(env);

		if (count <= 0) {
			return 1;
		}

		static auto do_range_insn = std::make_shared<Instruction>(
				exec_do_range,
				"EXEC.DO*RANGE",
				EXEC | INT,
				1);

		std::vector<std::shared_ptr<Code>> rcall{
				std::make_shared<Literal<int>>(0),
				std::make_shared<Literal<int>>(count),
				do_range_insn, code
		};

		get_exec_stack(env).push_back(std::make_shared<CodeList>(rcall));
	}
	return 1;
}

inline unsigned exec_do_times(Env& env) {
	auto exec_stack_size = get_exec_stack(env).size();
	auto int_stack_size = get_stack<int>(env).size();

	if (exec_stack_size > 0 && int_stack_size > 0) {
		int count = pop<int>(env);
		auto code = pop_exec(env);

		if (count <= 0) {
			return 1;
		}

		static auto do_range_insn = std::make_shared<Instruction>(
				exec_do_range,
				"EXEC.DO*RANGE",
				EXEC | INT,
				1);
		static auto int_pop = std::make_shared<Instruction>(
				protected_pop<int>,
				"INTEGER.POP",
				INT);

		std::vector<std::shared_ptr<Code>> pop_code{int_pop, code};

		std::vector<std::shared_ptr<Code>> rcall{
				std::make_shared<Literal<int>>(0),
				std::make_shared<Literal<int>>(count),
				do_range_insn, std::make_shared<CodeList>(pop_code)
		};

		get_exec_stack(env).push_back(std::make_shared<CodeList>(rcall));
	}
	return 1;
}

inline unsigned exec_if(Env& env) {
	if (get_exec_stack(env).size() >= 2 && get_stack<bool>(env).size() > 0) {
		auto first = pop_exec(env);
		auto second = pop_exec(env);
		if (pop<bool>(env)) {
			get_exec_stack(env).push_back(first);
		} else {
			get_exec_stack(env).push_back(second);
		}
	}
	return 1;
}

inline unsigned exec_k(Env& env) {
	if (get_exec_stack(env).size() >= 2) {
		auto first = pop_exec(env);
		auto second = pop_exec(env);
		get_exec_stack(env).push_back(first);
	}
	return 1;
}

inline unsigned exec_s(Env& env) {
	auto& stack = get_exec_stack(env);
	if (stack.size() >= 3) {
		auto a = pop_exec(env);
		auto b = pop_exec(env);
		auto c = pop_exec(env);

		std::vector<std::shared_ptr<Code>> bc{b, c};

		stack.push_back(std::make_shared<CodeList>(bc));
		stack.push_back(c);
		stack.push_back(a);
	}
	return 1;
}

inline unsigned exec_y(Env& env) {
	if (get_exec_stack(env).size() > 0) {
		auto first = pop_exec(env);
		static auto y_insn = std::make_shared<Instruction>(
				exec_y,
				"EXEC.Y",
				EXEC,
				1);

		std::vector<std::shared_ptr<Code>> rcall{y_insn, first};
		get_exec_stack(env).push_back(std::make_shared<CodeList>(rcall));
		get_exec_stack(env).push_back(first);
	}
	return 1;
}

} // namespace cppush

#endif // EXEC_OPS_H
