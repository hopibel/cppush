#ifndef CODE_H
#define CODE_H

#include <memory>
#include <string>
#include <vector>

namespace cppush {

class Env;

// TODO(hopibel): write an instruction library class for easy filtering and creation of instruction sets

class Code {
	public:
		Code() {}
		Code(const std::vector<std::shared_ptr<Code>>& stack) : stack_(stack) {}

		virtual ~Code() {}

		bool operator==(const Code& rhs) const;

		const std::vector<std::shared_ptr<Code>>& get_stack() const {return stack_;}

		virtual bool is_atom() const = 0;
		bool is_list() const {return !is_atom();}

		virtual unsigned operator()(Env& env) const = 0;
		virtual unsigned size() const = 0;

	protected:
		// operator== already checked CodeList stacks at this point, so they must be equal
		virtual bool equal_to(const Code&) const {return true;}

	private:
		friend class CodeList;
		std::vector<std::shared_ptr<Code>> stack_;
};

class CodeList : public Code {
	public:
		CodeList() {}
		CodeList(const std::vector<std::shared_ptr<Code>>& stack) : Code(stack) {}

		bool is_atom() const {return false;}
		unsigned operator()(Env& env) const override;
		unsigned size() const override;
};

class CodeAtom : public Code {
	public:
		bool is_atom() const {return true;}
		unsigned size() const override {return 1;}
};

} // namespace cppush

#endif // CODE_H
