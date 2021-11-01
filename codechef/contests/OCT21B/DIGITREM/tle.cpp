#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)a.size();
}

template <class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template <class T>
inline std::ostream &print(const T &x) {
  return print_one(x, '\n');
}
template <typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}

template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct VersatileInput {
  template <typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template <typename T>
    operator T() const {
      T x(n);
      for (auto &e : x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#define cerr \
  if (false) std::cerr
#endif

using namespace std;

template <typename T>
T power(T b, i64 e) {
  assert(e >= 0);
  T x = 1;
  while (e > 0) {
    if (e & 1) x *= b;
    b *= b;
    e >>= 1;
  }
  return x;
}

auto solve(i64 N, i64 d) -> i64 {
  // const i64 N = in, d = in;
  i64 n = N;
  while (true) {
    auto s = to_string(n);
    int p = -1;
    REP(i, ssize(s)) {
      if (s[i] == d + '0') {
        p = i;
        break;
      }
    }
    if (p == -1) {
      break;
    }
    int j = ssize(s) - 1 - p;
    i64 b = power(10, j);
    n = (n / b) * b + b;
    if (d == 0) {
      for (i64 z = 1; z < b; z *= 10) {
        n += z;
      }
    }
  }
  return n - N;
}

i64 brute(i64 n, i64 d) {
  auto check = [&]() {
    i64 t = n;
    while (t) {
      if (t % 10 == d) return false;
      t /= 10;
    }
    return true;
  };
  for (i64 i = 0;; ++i) {
    if (check()) return i;
    ++n;
  }
}

int main() {
  for (int n = 1; n <= 100000; ++n) {
    for (int d = 0; d <= 9; ++d) {
      i64 a = solve(n, d);
      i64 b = brute(n, d);
      if (a != b) {
        print(n, d, a, b);
        cout.flush();
      }
    }
  }
}