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
inline int popcount(unsigned x) { return __builtin_popcount(x); }

auto solve() {
  string N = in;
  int n = ssize(N);
  int m = in;
  vector<int> C = in(m);
  vector<int> rc(10, -1);
  REP(j, m) rc[C[j]] = j;
  const int kFull = (1 << m) - 1;

  auto dp = vector(n + 1, vector(1 << m, vector(2, pair<Mint, int>())));

  auto f = [&](auto &f, int i, int mask, bool down) -> optional<Mint> {
    if (i == n) {
      if (down or mask != 0) return nullopt;
      return 0;
    }
    if (dp[i][mask][down].second == 2) {
      return dp[i][mask][down].first;
    }
    if (dp[i][mask][down].second == 1) {
      return nullopt;
    }
    bool ret_ok = false;
    Mint ret = 0;
    int curd = N[n - 1 - i] - '0';
    bool down2 = false;
    if (down) {
      curd--;
      if (curd == -1) {
        curd = 9;
        down2 = true;
      }
    }
    //if (i < n - 1) {
    for (int d = 0; d <= 9; ++d) {
      bool down3 = down2;
      if (d > curd) {
        down3 = true;
      }
      int mask2 = mask;
      if (int j = rc[d]; j != -1) {
        mask2 = mask & ~(1 << j);
      }
      auto sub = f(f, i + 1, mask2, down3);
      if (sub.has_value()) {
        ret += *sub * 10 + d;
        ret_ok = true;
      }
    }
    //}

    // Head.
    [&]() {
      int headmax = 9;
      if (i == n - 1) {
        if (down2) return;
        headmax = curd;
      }
      for (int d = 1; d <= headmax; ++d) {
        int mask2 = mask;
        if (int j = rc[d]; j != -1) {
          mask2 = mask2 & ~(1 << j);
        }
        if (mask2 == 0) {
          ret += d;
          ret_ok = true;
        }
      }
    }();
    pair<Mint, int> result;
    if (ret_ok) {
      result.first = ret;
      result.second = 2;
      dp[i][mask][down] = result;
      return ret;
    }
    result.second = 1;
    dp[i][mask][down] = result;
    return nullopt;
  };

  auto ans = f(f, 0, kFull, false);
  return ans.value_or(0);
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
