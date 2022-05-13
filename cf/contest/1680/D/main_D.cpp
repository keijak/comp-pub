#include <bits/stdc++.h>
#include <cfenv>

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
void exit_() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

// Binary search over integers
template<class T, class F>
auto bisect(T truthy, T falsy, F pred) -> T {
  static_assert(std::is_integral_v<T>);
  static_assert(std::is_invocable_r_v<bool, F, T>);
  while (std::max(truthy, falsy) - std::min(truthy, falsy) > T(1)) {
    auto mid = (truthy & falsy) + (truthy ^ falsy) / T(2);
    auto ok = pred(mid);
    (ok ? truthy : falsy) = std::move(mid);
  }
  return truthy;
}

Int solve() {
  const int n = in;
  const Int K = in;
  vector<Int> a = in(n);
  int zeros = 0;
  REP(i, n) {
    if (a[i] == 0) ++zeros;
  }
  {
    Int asum = accumulate(ALL(a), 0LL);
    if (asum + K * zeros < 0) {
      return -1;
    }
    if (asum - K * zeros > 0) {
      return -1;
    }
  }

  Int ans = 1;
  REP(trial, 2) {
    vector<Int> zacc(n + 1), sacc(n + 1);
    REP(i, n) {
      zacc[i + 1] = zacc[i] + int(a[i] == 0);
      sacc[i + 1] = sacc[i] + a[i];
    }
    const Int asum = sacc[n];
    DUMP(zacc);
    DUMP(sacc);
    REP(l, n) {
      REP(r, l + 1, n + 1) {
        Int zcount = zacc[r] - zacc[l];
        Int sdelta = sacc[r] - sacc[l];
        Int dmin = -K * zcount, dmax = K * zcount;
        Int emin = -(zeros - zcount) * K - asum, emax = (zeros - zcount) * K - asum;
        if (emax < dmin or dmax < emin) continue;
        Int maxadd = min(dmax, emax);
        Int now = 1 + sdelta + maxadd;
        chmax(ans, now);
      }
    }
    REP(i, n) a[i] *= -1;
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { print(solve()); }
  exit_();
}
