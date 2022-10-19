// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBig = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

Mint choose2(int n) {
  static const Mint half = Mint(2).inv();
  if (n < 2) return 0;
  return Mint(n) * (n - 1) * half;
}

auto solve() -> Mint {
  const int n = in;
  vector<int> R = in(n), C = in(n);
  const int rsum = accumulate(ALL(R), 0);
  const int csum = accumulate(ALL(C), 0);
  if (rsum != csum) return 0;
  int c1 = 0, c2 = 0, r1 = 0, r2 = 0;
  REP(i, n) {
    if (C[i] == 1) ++c1;
    else if (C[i] == 2) ++c2;
    if (R[i] == 1) ++r1;
    else if (R[i] == 2) ++r2;
  }
  DUMP(c1, c2, r1, r2);
  auto dp = vector(r1 + r2 + 1, vector(c2 + 1, Mint(0)));
  dp[0][0] = 1;
  REP(i, r1) {
    REP(j, c2 + 1) {
      Mint now = dp[i][j];
      if (now.val() == 0) continue;
      const int rem_c1 = c1 - i + j * 2;
      const int rem_c2 = c2 - j;
      if (rem_c1 < 0) continue;
      dp[i + 1][j] += now * rem_c1;
      if (j + 1 <= c2) dp[i + 1][j + 1] += now * rem_c2;
    }
  }
  REP(i, r2) {
    REP(j, c2 + 1) {
      Mint now = dp[r1 + i][j];
      if (now.val() == 0) continue;
      const int rem_c1 = c1 - (r1 + i * 2) + j * 2;
      const int rem_c2 = c2 - j;
      if (rem_c1 < 0) continue;
      dp[r1 + i + 1][j] += now * choose2(rem_c1);
      if (j + 1 <= c2) {
        dp[r1 + i + 1][j + 1] += now * (rem_c1 + 1) * rem_c2;
      }
      if (j + 2 <= c2) {
        dp[r1 + i + 1][j + 2] += now * choose2(rem_c2);
      }
    }
  }
  return dp[r1 + r2][c2];
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    out(solve());
  }
  exit_();
}
