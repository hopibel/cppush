#ifndef TYPES_H
#define TYPES_H

namespace cppush {

enum class Type {
	EXEC =	0b0000'0001,
	CODE =	0b0000'0010,
	BOOL =	0b0000'0100,
	INT =	0b0000'1000,
	FLOAT =	0b0001'0000,
};

class Types {
	// Hack to allow initialization with 0 but not any other int (adapted from QFlags).
	// Zero is a pointer to a non-const int member of Private, which has no members,
	// so the only valid pointer would be nullptr (0).
	struct Private;
	using Zero = int (Private::*);

public:
	Types(Type t) : i(static_cast<int>(t)) {}
	Types(Zero = nullptr) : i(0) {} // no bits set

	Types operator|(const Types& other) const { return Types(Type(i | other.i)); }
	Types operator|(const Type& other) const { return Types(Type(i | static_cast<int>(other))); }
	bool operator==(const Types& other) const { return i == other.i; }
	bool operator==(const Type& other) const { return i == static_cast<int>(other); }

private:
	int i;
};

inline Types operator|(const Type& lhs, const Types& rhs) { return rhs | lhs; }
inline Types operator|(const Type& lhs, const Type& rhs) { return Types(lhs) | rhs; }
inline bool operator==(const Type& lhs, const Types& rhs) { return rhs == lhs; }

} // namespace cppush

#endif // TYPES_H
