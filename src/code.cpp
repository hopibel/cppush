#include "code.h"
#include "env.h"
#include "util.h"

#include <string>
#include <typeinfo>

namespace cppush {

unsigned Literal::operator()(Env& env) const {
	std::visit(overloaded{
		[&](bool arg) { env.push<bool>(arg); },
		[&](int arg) { env.push<int>(arg); },
		[&](double arg) { env.push<double>(arg); }
	}, value_);
	return 1;
}

unsigned CodeList::operator()(Env& env) const {
	for (auto it = list_.rbegin(); it < list_.rend(); ++it) {
		env.push<Exec>(*it);
	}
	return 1;
}

void CodeList::calc_size_() {
	size_ = 1;
	for (const auto& i : list_) {
		size_ += std::visit(overloaded{
			[&](CodeList arg) { return arg.size(); },
			[&](auto&&) -> unsigned { return 1; }
		}, i);
	}
}

std::string Literal::to_string() const {
	return std::visit(overloaded{
		[](auto&& arg) -> std::string { return std::to_string(arg); }
	}, value_);
}

// wrap string representation in parentheses
std::string CodeList::to_string() const {
	std::string output = "(";
	for (const auto& el : list_) {
		if (&el != &list_[0]) {
			output = output + " ";
		}
		output = output + std::visit(overloaded{
			[](auto&& arg) { return arg.to_string(); }
		}, el);
	}
	output += ")";
	return output;
}

std::ostream& operator<<(std::ostream& os, const Code& value) {
	os << std::visit(overloaded{
		[](auto&& arg) { return arg.to_string(); }
	}, value);
	return os;
}

} // namespace cppush
