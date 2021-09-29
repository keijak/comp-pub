#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <boost/multiprecision/cpp_int.hpp>
// using i128 = boost::multiprecision::checked_int128_t;
// using u128 = boost::multiprecision::checked_uint128_t;
using Int = boost::multiprecision::checked_cpp_int;
// using Rational = boost::multiprecision::checked_cpp_rational;
namespace multip = boost::multiprecision;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

template<typename Container>
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

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// [1, x, x^2, x^3, ..., x^n]
template<typename T = Int>
std::vector<T> pow_seq(int n, int base = 2) {
  assert(n >= 0 and base > 0);
  std::vector<T> p(n + 1);
  p[0] = 1;
  for (int i = 0; i < n; ++i) {
    p[i + 1] = p[i] * base;
  }
  return p;
}

std::string toBinary(Int n) {
  std::string r;
  while (n != 0) {
    r = (n % 2 == 0 ? "0" : "1") + r;
    n /= 2;
  }
  return r;
}

Int sub(int w) {
  if (w == 0) return 1;
  return sub(w - 1) * 2;
}

auto solve() -> string {
  const int n = in;
  const string X = in;
  Int x = 0;
  for (auto d: X) {
    x <<= 1;
    x += int(d - '0');
  }
  DUMP(X);
  DUMP(x);
  --x; // 0-indexed
  string ret(1, '1');
  int ones = 1;
  Int p2 = Int(1) << (n - 1);
  while (x != 0) {
    if (x < p2) {
      ret.push_back('0');
      x -= 1;
    } else {
      ret.push_back('1');
      x -= p2;
    }
    p2 >>= 1;
  }
  return ret;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
