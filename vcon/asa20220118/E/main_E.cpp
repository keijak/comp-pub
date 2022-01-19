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

// Safe (a+b)/2 on integers
template<class T>
T midpoint(T a, T b) {
  using U = std::make_unsigned_t<T>;
  int sign = 1;
  U m = a, M = b;
  if (a > b) sign = -1, m = b, M = a;
  return a + sign * T(U(M - m) >> 1);
}

template<class Float, class F>
Float bisect_float(Float truthy, Float falsy, F pred) {
  static_assert(std::is_floating_point_v<Float>);
  static_assert(std::is_invocable_r_v<bool, F, Float>);
  for (int iter = 0; iter < 80; ++iter) {
    auto mid = (truthy + falsy) * 0.5;
    if (pred(mid)) {
      truthy = std::move(mid);
    } else {
      falsy = std::move(mid);
    }
  }
  return truthy;
}

auto solve() {
  int n = in;
  vector<array<Int, 3>> p(n);
  REP(i, n) {
    Int x = in, y = in, c = in;
    p[i] = {x, y, c};
  }
  return bisect_float<Real>(1e16, -1, [&](Real t) {
    auto[x0, y0, c0] = p[0];
    Real xl = x0 - t / c0;
    Real xr = x0 + t / c0;
    Real yl = y0 - t / c0;
    Real yr = y0 + t / c0;
    REP(i, 1, n) {
      auto[xi, yi, ci] = p[i];
      chmax(xl, xi - t / ci);
      chmin(xr, xi + t / ci);
      chmax(yl, yi - t / ci);
      chmin(yr, yi + t / ci);
    }
    return (xl <= xr and yl <= yr);
  });
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
