#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) {
  return print_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

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

template<typename T,
    typename = std::enable_if_t<is_iterable<T>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct VersatileInput {
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

int64_t mod_log(int64_t a, int64_t b, int64_t p) {
  int64_t g = 1;

  for (int64_t i = p; i; i /= 2) (g *= a) %= p;
  g = std::gcd(g, p);

  int64_t t = 1, c = 0;
  for (; t % g; c++) {
    if (t == b) return c;
    (t *= a) %= p;
  }
  if (b % g) return -1;

  t /= g;
  b /= g;

  int64_t n = p / g, h = 0, gs = 1;

  for (; h * h < n; h++) (gs *= a) %= n;

  unordered_map<int64_t, int64_t> bs;
  for (int64_t s = 0, e = b; s < h; bs[e] = ++s) {
    (e *= a) %= n;
  }

  for (int64_t s = 0, e = t; s < n;) {
    (e *= gs) %= n;
    s += h;
    if (bs.count(e)) return c + s - bs[e];
  }
  return -1;
}

#include <atcoder/math>

auto solve(i64 K) -> i64 {
  // if (K == 1 or K == 2) return 1;
  if (K % 2 == 0) {
    K /= 2;
  }
  if (K % 3 == 0) {
    K *= 9;
  }
  if (gcd(10, K) != 1) {
    return -1LL;
  }
  i64 q = atcoder::inv_mod(10, K);
  i64 res = mod_log(10LL, q, K);
  return res + 1;
}

#include <boost/multiprecision/cpp_int.hpp>
using Int = boost::multiprecision::checked_cpp_int;
namespace multip = boost::multiprecision;

i64 brute(i64 K) {
  Int x = 2;
  for (int i = 1; i <= 1000; ++i) {
    if (x % K == 0) return i;
    x = 10 * x + 2;
  }
  return -1LL;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  for (int i = 1; i <= 100; ++i) {
    i64 a = solve(i);
    i64 b = brute(i);
    if (a != b) {
      print(i, a, b);
      cout.flush();
    }
    print(i, "ok");
  }
}