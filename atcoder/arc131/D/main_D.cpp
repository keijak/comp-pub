#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;

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

template<class T>
T floor_div(T x, T y) {
  assert(y != 0);
  return x / y - (((x < 0) ^ (y < 0)) and (x % y));
}

template<class T>
T ceil_div(T x, T y) {
  assert(y != 0);
  return x / y + (not((x < 0) ^ (y < 0)) and (x % y));
}

auto solve() {
  Int n = in, m = in, D = in;
  vector<Int> R = in(m + 1);
  vector<Int> S = in(m);
  assert(R[0] == 0);

  Int L = -(n / 2) * D;
  vector<Int> d_fast(D + 100);
  vector<Int> d_naive(D + 100);
  Int base_score = 0;
  DUMP(L);
  for (Int i = -m; i < m; ++i) {
    Int l = (i <= 0) ? -R[-i] : R[i] + 1;
    Int r = (i < 0) ? -R[-(i + 1)] - 1 : R[i + 1];
    assert(l <= r);
    Int score = (i < 0) ? S[-(i + 1)] : S[i];
    Int kl = max<Int>(ceil_div<Int>(l - L, D), 0);
    Int kr = min<Int>(floor_div<Int>(r - L, D), n - 1);
    Int xl = L + kl * D;
    Int xr = L + kr * D;
#if 1
    if (0 <= kl and kl <= kr and kr <= n - 1) {
      Int c0 = kr - kl + 1;
      base_score += c0 * score;
    }
    if (0 <= kr and kr <= n - 1 and xr <= r and xr + D > r) {
      Int t1 = r - xr + 1;
      if (0 <= t1 and t1 < D) {
        d_fast[t1] -= score;
      }
    }
    if (0 <= kl - 1 and kl - 1 <= n - 1 and xl - D < l and l <= xl) {
      Int t2 = l - (xl - D);
      if (0 <= t2 and t2 < D) {
        d_fast[t2] += score;
      }
    }
#else
    // naive
    for (Int d = 0; d < D; ++d) {
      Int cnt = 0;
      REP(j, n) {
        Int x = L + j * D + d;
        if (l <= x and x <= r) ++cnt;
      }
      d_naive[d] += score * cnt;
    }
#endif
  }
  for (int d = 0; d < D; ++d) {
    d_fast[d + 1] += d_fast[d];
  }
  Int ans = 0;
  Int opt_d = -1;
  for (int d = 0; d < D; ++d) {
    if (chmax(ans, base_score + d_fast[d] + d_naive[d])) {
      opt_d = d;
    }
  }
  DUMP(opt_d);
  print(ans);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    (solve());
  }
}
