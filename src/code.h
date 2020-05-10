#ifndef CODE_H
#define CODE_H

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

namespace cppush {

class Code;
class Env;

using Code_ptr = std::shared_ptr<Code>;

class Code {
public:
	Code() {}
	Code(const std::vector<Code_ptr>& stack) : stack_(stack) {}

	virtual ~Code() {}

	bool operator==(const Code& rhs) const;

	const std::vector<Code_ptr>& get_stack() const {return stack_;}

	virtual bool is_atom() const = 0;
	bool is_list() const {return !is_atom();}

	virtual unsigned operator()(Env& env) const = 0;
	virtual unsigned size() const = 0;

protected:
	// operator== already checked CodeList stacks at this point, so they must be equal
	virtual bool equal_to(const Code&) const {return true;}

private:
	friend class CodeList;
	const std::vector<Code_ptr> stack_;
};

class CodeList : public Code {
public:
	CodeList() : size_(1) {}
	CodeList(const std::vector<Code_ptr>& stack) : Code(stack) {calc_size_();}

	bool is_atom() const {return false;}
	unsigned operator()(Env& env) const override;
	unsigned size() const override {return size_;}

private:
	unsigned size_;
	void calc_size_();
};

class CodeAtom : public Code {
public:
	bool is_atom() const {return true;}
	unsigned size() const override {return 1;}
};

} // namespace cppush

#endif // CODE_H
