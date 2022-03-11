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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<class T>
T saturating_add(T x, T y) {
  static_assert(std::is_integral<T>::value);
  static constexpr T kMin = std::numeric_limits<T>::lowest();
  static constexpr T kMax = std::numeric_limits<T>::max();
  static_assert(kMax != 0);
  if (T res; not __builtin_add_overflow(x, y, &res)) {
    return res;
  } else if constexpr (not std::is_signed<T>::value) {
    return kMax;
  } else {
    return (x < 0) ? kMin : kMax;
  }
}

template<class T>
T saturating_mul(T x, T y) {
  static_assert(std::is_integral<T>::value);
  static constexpr T kMin = std::numeric_limits<T>::lowest();
  static constexpr T kMax = std::numeric_limits<T>::max();
  static_assert(kMax != 0);
  if (T res; not __builtin_mul_overflow(x, y, &res)) {
    return res;
  } else if constexpr (not std::is_signed<T>::value) {
    return kMax;
  } else {
    return ((x ^ y) < 0) ? kMin : kMax;
  }
}

template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

auto solve() {
  int n = in;
  Int K = in;
  vector<Int> f(n + 100), g(n + 100);
  f[n - 1] = 1;
  g[n - 1] = 0;
  for (int i = n - 2; i >= 0; --i) {
    f[i] = saturating_add(saturating_mul(f[i + 1], 2LL), 1LL);
    g[i] = saturating_add(saturating_mul(g[i + 1], 2LL), f[i + 1]);
  }
  auto h = Rec([&](auto &h, Int x, int i) -> Int {
    if (x <= 1) return 0;
    if (x <= 1 + f[i + 1]) {
      return h(x - 1, i + 1);
    }
    Int x2 = x - 1 - f[i + 1];
    return x2 + g[i + 1] + h(x2, i + 1);
  });
  return K + h(K, 0);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
