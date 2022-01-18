#include <bits/stdc++.h>
#include <atcoder/math>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

std::optional<Int> sqrt_mod(Int a, int p) {
  if (a == 0) return 0;
  if (p == 2) return a;
  if (atcoder::pow_mod(a, (p - 1) >> 1, p) != 1) return std::nullopt;
  Int b = 1;
  while (atcoder::pow_mod(b, (p - 1) >> 1, p) == 1) ++b;
  Int e = 0, m = p - 1;
  while (m % 2 == 0) m >>= 1, ++e;
  auto x = atcoder::pow_mod(a, (m - 1) >> 1, p);
  auto y = a * (x * x % p) % p;
  (x *= a) %= p;
  auto z = atcoder::pow_mod(b, m, p);
  while (y != 1) {
    Int j = 0, t = y;
    while (t != 1) {
      j += 1;
      (t *= t) %= p;
    }
    z = atcoder::pow_mod(z, 1LL << (e - j - 1), p);
    (x *= z) %= p;
    (z *= z) %= p;
    (y *= z) %= p;
    e = j;
  }
  return x;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  const int T = in;
  REP(t, T) {
    const int y = in, p = in;
    print(sqrt_mod(y, p).value_or(-1));
  }
}
