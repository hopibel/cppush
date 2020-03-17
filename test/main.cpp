#include "code.h"
#include "env.h"
#include "instruction.h"
#include "numeric_ops.h"

#include <iostream>
#include <memory>

unsigned test(cppush::Env& env) {
	if (env.int_stack.size() >= 1) {
		env.int_stack.push_back(env.int_stack.back());
	} else {
		env.int_stack.push_back(42);
	}
	return 1;
}

int main(void) {
	cppush::Env env;

	std::shared_ptr<cppush::Instruction> noop = std::make_shared<cppush::Instruction>(
			cppush::sub<int>, "INTEGER.-");
	std::vector<std::shared_ptr<cppush::Code>> code_block;

	code_block.push_back(noop);
	code_block.push_back(noop);
	code_block.push_back(noop);

	env.int_stack.push_back(70);
	env.int_stack.push_back(1);

	env.load_program(std::make_shared<cppush::CodeList>(code_block));
	for (auto i : env.int_stack) {
		std::cout << i << " ";
	}
	std::cout << "\n";
	env.run();
	std::cout << noop->name << "\n";
	for (auto i : env.int_stack) {
		std::cout << i << " ";
	}
	std::cout << "\n";

	std::shared_ptr<cppush::Instruction> sub2 = std::make_shared<cppush::Instruction>(
			cppush::sub<int>, "INTEGER.-");
	std::cout << (*sub2 == *noop ? "equal" : "not equal") << "\n";

	return 0;
}
