#pragma once

#include <iostream>
#include <tuple>
#include <utility>

std::ostream &operator<<(std::ostream &os, const __uint128_t &x) {
  if (x > 9) os << (x / 10);
  return os << (x % 10 + '0');
}

std::ostream &operator<<(std::ostream &os, const __int128_t &x) {
  __uint128_t y;
  if (x < 0) {
    os << '-';
    y = -x;
  } else {
    y = x;
  }
  return os << y;
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

namespace aux {
template <std::size_t...>
struct seq {};

template <std::size_t N, std::size_t... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

template <std::size_t... Is>
struct gen_seq<0, Is...> : seq<Is...> {};

template <class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple(std::basic_ostream<Ch, Tr> &os, Tuple const &t, seq<Is...>) {
  using swallow = int[];
  (void)swallow{0,
                (void(os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), 0)...};
}
}  // namespace aux

template <class Ch, class Tr, class... Args>
auto operator<<(std::basic_ostream<Ch, Tr> &os, std::tuple<Args...> const &t)
    -> std::basic_ostream<Ch, Tr> & {
  os << "(";
  aux::print_tuple(os, t, aux::gen_seq<sizeof...(Args)>());
  return os << ")";
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
