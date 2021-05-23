#ifndef CODE_H
#define CODE_H

#include <memory>
#include <vector>

namespace cppush {

class State;
using Op = unsigned (*)(State&);

class Code {
public:
	virtual ~Code() = default;

	virtual unsigned operator()(State&) = 0;
};

class Instruction : public Code {
public:
	Instruction(Op);
	~Instruction() override {}
	unsigned operator()(State&) override;

private:
	Op op;
};

class CodeList : public Code {
public:
	CodeList(std::vector<std::shared_ptr<Code>>);
	~CodeList() override {}
	unsigned operator()(State&) override;

private:
	std::vector<std::shared_ptr<Code>> vec;
};

} // namespace cppush

/*
class Env;

class Instruction;
class Literal;
class CodeList;
using Code = std::variant<Instruction, Literal, CodeList>;

using literal_t = std::variant<bool, int, double>;

class Instruction {
public:
	// parens is number of blocks to open after an instruction in a genome
	Instruction(unsigned (*op)(Env&), std::string name, unsigned parens = 0) :
		op(op), name(name), parens(parens) {}

	unsigned operator()(Env& env) const { return op(env); }
	bool operator==(const Instruction& rhs) const { return op == rhs.op; }

	std::string to_string() const { return name; }
	unsigned get_parens() const { return parens; }

private:
	unsigned (*op)(Env&);
	std::string name;
	unsigned parens;
};

class Literal {
public:
	Literal(bool value) : value(value) {}
	Literal(int value) : value(value) {}
	Literal(double value) : value(value) {}

	unsigned operator()(Env& env) const;
	bool operator==(const Literal& rhs) const { return value == rhs.value; }

	literal_t get() const { return value; }
	std::string to_string() const;

private:
	literal_t value;
};

class CodeList {
public:
	CodeList() : size_(1) {}
	CodeList(const std::vector<Code>& list) : list_(list) { calc_size_(); }

	unsigned operator()(Env& env) const;
	bool operator==(const CodeList& rhs) const { return list_ == rhs.list_; }
	bool operator!=(const CodeList& rhs) const { return !(*this == rhs); }

	const std::vector<Code>& get_list() const { return list_; }
	unsigned size() const { return size_; }
	std::string to_string() const;

private:
	std::vector<Code> list_;
	unsigned size_;

	void calc_size_();
};

inline bool is_list(const Code& code) {
	return std::visit(overloaded{
		[](CodeList) { return true; },
		[](auto&&) { return false; }
	}, code);
}
inline bool is_atom(const Code& code) { return !is_list(code); }
inline unsigned size(const Code& code) {
	return std::visit(overloaded{
		[](CodeList arg) -> unsigned { return arg.size(); },
		[](auto&&) -> unsigned { return 1; }
	}, code);
}

// string conversion
std::ostream& operator<<(std::ostream& os, const Code& value);

} // namespace cppush

#include "env.h"
*/

#endif // CODE_H
