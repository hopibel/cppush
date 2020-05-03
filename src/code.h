#ifndef CODE_H
#define CODE_H

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

namespace cppush {

class CodeBase;
class Env;

using Code = std::shared_ptr<CodeBase>;

class CodeBase {
	public:
		CodeBase() {}
		CodeBase(const std::vector<Code>& stack) : stack_(stack) {}

		virtual ~CodeBase() {}

		bool operator==(const CodeBase& rhs) const;

		const std::vector<Code>& get_stack() const {return stack_;}

		virtual bool is_atom() const = 0;
		bool is_list() const {return !is_atom();}

		virtual unsigned operator()(Env& env) const = 0;
		virtual unsigned size() const = 0;

	protected:
		// operator== already checked CodeList stacks at this point, so they must be equal
		virtual bool equal_to(const CodeBase&) const {return true;}

	private:
		friend class CodeList;
		const std::vector<Code> stack_;
};

class CodeList : public CodeBase {
	public:
		CodeList() : size_(1) {}
		CodeList(const std::vector<Code>& stack) : CodeBase(stack) {calc_size_();}

		bool is_atom() const {return false;}
		unsigned operator()(Env& env) const override;
		unsigned size() const override {return size_;}

	private:
		unsigned size_;
		void calc_size_();
};

class CodeAtom : public CodeBase {
	public:
		bool is_atom() const {return true;}
		unsigned size() const override {return 1;}
};

} // namespace cppush

#endif // CODE_H
