#ifndef LITERAL_BASE_H
#define LITERAL_BASE_H

namespace cppush {

class State;

// TODO: string conversion
class LiteralBase {
public:
	virtual ~LiteralBase() = default;
	virtual void exec(State state) = 0;
};

} // namespace cppush

#endif // LITERAL_BASE_H