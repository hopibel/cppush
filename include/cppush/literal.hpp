#ifndef LITERAL_H
#define LITERAL_H

#include "literal_base.hpp"
#include "state.hpp"

namespace cppush {

template <typename T>
class Literal : public LiteralBase {
public:
	Literal(T value) : value(value) {}
	virtual void exec(State state) { state.get_stack<T>().push_back(value); }
private:
	T value;
};

} // namespace cppush

#endif // LITERAL_H