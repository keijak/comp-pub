#include <bits/stdc++.h>
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
void exit() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

#include <boost/multiprecision/cpp_int.hpp>
using BigInt = boost::multiprecision::int128_t;
namespace multip = boost::multiprecision;

// Binary search over integers
template<class T, class F>
auto bisect(T truthy, T falsy, F pred) -> T {
  //static_assert(std::is_integral_v<T>);
  static_assert(std::is_invocable_r_v<bool, F, T>);
  while (std::max(truthy, falsy) - std::min(truthy, falsy) > T(1)) {
    auto mid = (truthy & falsy) + (truthy ^ falsy) / T(2);
    auto ok = pred(mid);
    (ok ? truthy : falsy) = std::move(mid);
  }
  return truthy;
}

constexpr int L = 1000'000;

BigInt solve() {
  Int N = in;
  if (N == 0) return 0;
  BigInt X = kBigVal<Int>;
  for (BigInt a = 0; a <= L; ++a) {
    BigInt a3 = a * a * a;
    if (a3 > N) break;
    if (a3 == N) {
      X = N;
      break;
    }
    BigInt z = bisect(BigInt(L), BigInt(0), [&](const BigInt &b) -> bool {
      return a3 + a * a * b + a * b * b + b * b * b >= N;
    });
    chmin(X, a3 + a * a * z + a * z * z + z * z * z);
    assert(X >= N);
  }
  return X;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { print(solve()); }
  exit();
}
