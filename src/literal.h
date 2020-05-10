#ifndef LITERAL_H
#define LITERAL_H

#include "code.h"

// declarations
namespace cppush {

class Env;

template <typename T>
class Literal : public CodeAtom {
public:
	Literal(T val);
	unsigned operator()(Env& env) const override;

protected:
	bool equal_to(const Code& rhs) const override;

private:
	T value_;
};

} // namespace cppush

#include "env.h"

// definitions
namespace cppush {

template <typename T>
Literal<T>::Literal(T val) : value_(val) {}

template <typename T>
unsigned Literal<T>::operator()(Env& env) const {
	env.push<T>(value_);
	return 1;
}

template <typename T>
bool Literal<T>::equal_to(const Code& rhs) const {
	return value_ == static_cast<const Literal<T>&>(rhs).value_;
}

} // namespace cppush

#endif // LITERAL_H
