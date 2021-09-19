#pragma once

#include <iostream>
#include <tuple>
#include <utility>

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename T1, typename T2, typename T3>
std::ostream &operator<<(std::ostream &os, const std::tuple<T1, T2, T3> &a) {
  const auto &[x1, x2, x3] = a;
  return os << "(" << x1 << ", " << x2 << ", " << x3 << ")";
}

template <typename T>
void dump_(const T &value) {
  std::cerr << value;
}

template <typename T, typename... Ts>
void dump_(const T &value, const Ts &...args) {
  dump_(value);
  std::cerr << ", ";
  dump_(args...);
}

#define DUMP(...)                                    \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    dump_(__VA_ARGS__);                              \
    std::cerr << std::endl;                          \
  } while (0)
