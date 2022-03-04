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
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

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

void solve() {
  int n = in;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int u = in, v = in;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  if (n == 2) {
    print(2, 2);
    print(1, 1);
    return;
  }

  auto dp = vector(n, array<pair<int, Int>, 2>());

  auto dfs = Rec([&](auto &dfs, int v, int p) -> void {
    array<pair<int, Int>, 2> res = {pair{1, -ssize(g[v])}, pair{0, -1LL}};
    for (auto u: g[v]) {
      if (u == p) continue;
      dfs(u, v);
      auto sub = dp[u];
      res[0].first += sub[1].first;
      res[0].second += sub[1].second;
      auto ss = max(sub[0], sub[1]);
      res[1].first += ss.first;
      res[1].second += ss.second;
    }
    dp[v] = res;
  });
  dfs(0, -1);
  auto dpo = max(dp[0][0], dp[0][1]);
  print(dpo.first, -dpo.second);

  auto w = vector(2, vector(n, 0LL));

  auto dfs2 = Rec([&](auto &dfs2, int v, int p, int bad, int j) -> bool {
    if (bad) {
      w[j][v] = 1;
    } else {
      w[j][v] = ssize(g[v]);
    }
    for (auto u: g[v]) {
      if (u == p) continue;
      bool ubad = (not bad) or (dp[u][0] < dp[u][1]);
      dfs2(u, v, ubad, j);
    }
    return true;
  });

  dfs2(0, -1, 0, 0);
  dfs2(0, -1, 1, 1);
  if (dp[0][0] >= dp[0][1]) {
    print_seq(w[0]);
  } else {
    print_seq(w[1]);
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    solve();
  }
}
