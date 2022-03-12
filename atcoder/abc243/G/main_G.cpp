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

// Returns floor(sqrt(x)).
Int floor_sqrt(Int x) {
  assert(x >= 0);
  if (x <= 1) return x;
  Int r = std::floor(sqrtl((long double) x));
  while (r * r < x) ++r;
  while (r * r > x) --r;
  return r;
}

struct Solver {
  static constexpr int L = 54775;
  vector<Int> f, fsum;
  Solver() : f(L), fsum(L) {
    f[1] = 1;
    fsum[1] = 1;
    for (int i = 2; i < L; ++i) {
      Int r = floor_sqrt(i);
      for (int j = 1; j <= r; ++j) {
        f[i] += f[j];
      }
      fsum[i] = fsum[i - 1] + f[i];
    }

  }

  auto solve() {
    Int X = in;
    if (X < L) {
      return f[X];
    }

    Int rx = floor_sqrt(X);
    if (rx < L) {
      return fsum[rx];
    }

    Int ans = 0;
    for (Int i = 1; i * i <= rx; ++i) {
      Int p = (i + 1) * (i + 1) - 1;
      Int co = min<Int>(p, rx) - (i * i - 1);
      ans += co * fsum[i];
    }
    return ans;
  }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  Solver solver;
  const int T = in;
  REP(t, T) {
    print(solver.solve());
  }
}
