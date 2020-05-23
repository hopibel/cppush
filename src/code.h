#ifndef CODE_H
#define CODE_H

#include "util.h"

#include <string>
#include <variant>
#include <vector>

namespace cppush {

class Env;

class Instruction;
class Literal;
class CodeList;
using Code = std::variant<Instruction, Literal, CodeList>;

using literal_t = std::variant<bool, int, double>;

class Instruction {
public:
	Instruction(unsigned (*op)(Env&), std::string name) : op_(op), name_(name) {}

	unsigned operator()(Env& env) const { return op_(env); }
	bool operator==(const Instruction& rhs) const { return op_ == rhs.op_; }

	std::string to_string() const { return name_; }

private:
	unsigned (*op_)(Env&);
	std::string name_;
};

class Literal {
public:
	Literal(bool value) : value_(value) {}
	Literal(int value) : value_(value) {}
	Literal(double value) : value_(value) {}

	unsigned operator()(Env& env) const;
	bool operator==(const Literal& rhs) const { return value_ == rhs.value_; }

	std::string to_string() const;

private:
	literal_t value_;
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

#endif // CODE_H
