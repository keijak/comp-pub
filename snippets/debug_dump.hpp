#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

std::ostream &operator<<(std::ostream &os, const __uint128_t &x) {
  if (x >= 10) os << static_cast<__uint128_t>(x / 10);
  return os << static_cast<char>(x % 10 + '0');
}

std::ostream &operator<<(std::ostream &os, const __int128_t &x) {
  __uint128_t y = x;
  if (x < 0) {
    os << '-';
    y = -x;
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

template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename Container>
std::ostream &print_seq(const Container &a, const char *sep, const char *ends,
                        std::ostream &os) {
  auto itl = std::begin(a), itr = std::end(a);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

}  // namespace aux

template <class Ch, class Tr, class... Args>
auto operator<<(std::basic_ostream<Ch, Tr> &os, std::tuple<Args...> const &t)
    -> std::basic_ostream<Ch, Tr> & {
  os << "(";
  ::aux::print_tuple(os, t, aux::gen_seq<sizeof...(Args)>());
  return os << ")";
}

template <typename T,
          typename = std::enable_if_t<::aux::is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return ::aux::print_seq(a, ", ", "", (os << "{")) << "}";
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
