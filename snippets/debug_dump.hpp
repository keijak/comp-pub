#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace aux {
template<std::size_t...>
struct indices {};

template<std::size_t N, std::size_t... Ix>
struct gen_indices : gen_indices<N - 1, N - 1, Ix...> {};

template<std::size_t... Ix>
struct gen_indices<0, Ix...> : indices<Ix...> {};

template<class Tuple, std::size_t... Ix>
void print_tuple(std::ostream &os, Tuple const &t, indices<Ix...>);

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename Container>
void print_seq(const Container &a, const char *sep, const char *ends,
               std::ostream &os);
}  // namespace aux

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

template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &p) {
  return os << '(' << p.first << ", " << p.second << ')';
}

template<class... Args>
std::ostream &operator<<(std::ostream &os, const std::tuple<Args...> &t) {
  os << '(';
  ::aux::print_tuple(os, t, aux::gen_indices<sizeof...(Args)>());
  return os << ')';
}

template<typename T,
    typename = std::enable_if_t<::aux::is_iterable<T>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  os << '{';
  ::aux::print_seq(a, ", ", "", os);
  return os << '}';
}

namespace aux {

template<class Tuple, std::size_t... Ix>
void print_tuple(std::ostream &os, Tuple const &t, indices<Ix...>) {
  using swallow = int[];
  swallow{0, (void(os << (Ix == 0 ? "" : ", ") << std::get<Ix>(t)), 0)...};
}

template<typename Container>
void print_seq(const Container &a, const char *sep, const char *ends,
               std::ostream &os) {
  auto itl = std::begin(a), itr = std::end(a);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  os << ends;
}

int next_comma_(const std::string& s) {
  int i = 0, stk = 0;;
  for (; i < (int)s.size(); ++i) {
    if (s[i] == ',') {
      if (stk == 0) break;
    } else if (s[i] == '(') {
      ++stk;
    } else if (s[i] == ')') {
      --stk;
    }
  }
  return i;
}

template<typename T>
void dump_(std::string name, const T &value) {
  std::cerr << "\033[32m" << name << "\033[33m=\033[0m" << value;
}

template<typename T, typename... Ts>
void dump_(std::string names, const T &value, const Ts &...args) {
  const int p = next_comma_(names);
  dump_(names.substr(0, p), value);
  std::cerr << "\033[33m; \033[0m";
  dump_(names.substr(p + 2), args...);
}
}  // namespace aux

#define DUMP(...)                                    \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ":";  \
    std::cerr << __FUNCTION__ << ")\033[0m ";        \
    ::aux::dump_(#__VA_ARGS__, __VA_ARGS__);         \
    std::cerr << std::endl;                          \
  } while (0)
