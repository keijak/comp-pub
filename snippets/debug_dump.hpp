#pragma once

#include <iostream>

namespace {
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
}  // namespace

#define DUMP(...)                                    \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    dump_(__VA_ARGS__);                              \
    std::cerr << std::endl;                          \
  } while (0)
