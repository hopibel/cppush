#ifndef LITERAL_H
#define LITERAL_H

#include "code.h"
#include "env.h"

namespace cppush {

template <typename T>
class Literal : public CodeAtom {
	public:
		Literal(T val) : value_(val) {}

		unsigned operator()(Env& env) const override {
			get_stack<T>(env).push_back(value_);
			return 1;
		}

	protected:
		inline bool equal_to(const Code& rhs) const override {
			return value_ == static_cast<const Literal<T>&>(rhs).value_;
		}

	private:
		T value_;
};

} // namespace cppush

#endif // LITERAL_H
