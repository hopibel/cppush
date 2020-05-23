#ifndef UTIL_H
#define UTIL_H

namespace cppush {

// TODO: profile and replace with handrolled switch implementation if needed
// std::visit helper. construct visitor from lambdas
template <typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;

} // namespace cppush

#endif // UTIL_H
