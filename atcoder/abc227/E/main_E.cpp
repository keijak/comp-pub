#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

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
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

struct Void {};  // No print.

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, Void>) {
    return std::cout;  // Nothing.
  } else if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
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
constexpr int M = 455;

auto solve() {
  string s = in;
  int K = in;
  chmin(K, M);
  int n = ssize(s);
  vector<int> cmap(256, -1);
  cmap['K'] = 0, cmap['E'] = 1, cmap['Y'] = 2;
  vector<int> a(n);
  vector<int> sz(3);
  vector<vector<int>> pos(3);
  REP(i, n) {
    a[i] = cmap[s[i]];
    check(a[i] >= 0);
    pos[a[i]].push_back(i);
    ++sz[a[i]];
  }
  REP(i, 3) DUMP(i, sz[i], pos[i]);
  auto dp = vector(sz[0] + 1, vector(sz[1] + 1, vector(sz[2] + 1, vector(K + 1, 0LL))));
  dp[0][0][0][0] = 1;
  array<int, 3> ii = {0, 0, 0};
  for (ii[0] = 0; ii[0] <= sz[0]; ++ii[0]) {
    for (ii[1] = 0; ii[1] <= sz[1]; ++ii[1]) {
      for (ii[2] = 0; ii[2] <= sz[2]; ++ii[2]) {
        REP(x, 3) {
          if (ii[x] == sz[x]) continue;
          int r = pos[x][ii[x]];
          int delta = 0;
          REP(y, 3) if (y != x) {
              REP(t, ii[y]) {
                if (pos[y][t] > r) ++delta;
              }
            }
          REP(inv, K + 1) {
            Int pv = dp[ii[0]][ii[1]][ii[2]][inv];
            int ninv = inv + delta;
            if (pv != 0 and ninv <= K) {
              ii[x]++;
              dp[ii[0]][ii[1]][ii[2]][ninv] += pv;
              ii[x]--;
            }
          }
        }
      }
    }
  }

  auto &t = dp[sz[0]][sz[1]][sz[2]];
  Int ans = 0;
  REP(k, K + 1) { ans += t[k]; }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
