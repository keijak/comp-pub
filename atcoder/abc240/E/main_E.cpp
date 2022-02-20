#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = b), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = b), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)a.size();
}
template <typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template <typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template <typename T>
inline std::ostream &print(const T &x) {
  return print_one(x, '\n');
}
template <typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template <typename Container>
std::ostream &print_seq(const Container &seq, const char *sep = " ",
                        const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template <typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template <typename T>
    operator T() const {
      T xs(n);
      for (auto &x : xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "backward.hpp"
#include "debug_dump.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr \
  if (false) cerr
#endif

using namespace std;

template <class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template <class... Args>
  decltype(auto) operator()(Args &&...args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template <class F>
Rec(F) -> Rec<F>;

auto solve() {
  int n = in;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int u = in, v = in;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }

  vector<pair<int, int>> lr(n);
  int leaf_cnt = 0;

  auto dfs = Rec([&](auto &dfs, int v, int p) -> pair<int, int> {
    bool is_leaf = p != -1 and (ssize(g[v]) == 1);
    if (is_leaf) {
      lr[v] = {leaf_cnt, leaf_cnt};
      leaf_cnt++;
      return lr[v];
    }
    int lmin = -1, rmax = -1;
    for (auto u : g[v]) {
      if (u == p) continue;
      auto [l, r] = dfs(u, v);
      if (lmin == -1) {
        lmin = l;
      }
      rmax = r;
    }
    assert(rmax >= 0);
    assert(rmax >= lmin);
    lr[v] = {lmin, rmax};
    return lr[v];
  });
  dfs(0, -1);
  DUMP(lr);
  REP(i, n) {
    auto [l, r] = lr[i];
    print(l + 1, r + 1);
  }
  return Void();
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;  // in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
