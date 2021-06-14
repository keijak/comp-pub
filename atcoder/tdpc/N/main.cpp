#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// mod: prime
template <class T = Mint>
struct Factorials {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  // n: max cached value.
  Factorials(int n) : fact(n + 1), ifact(n + 1) {
    assert(n >= 0);
    assert(n < T::mod());
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) {
      fact[i] = fact[i - 1] * i;
    }
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) {
      ifact[i - 1] = ifact[i] * i;
    }
  }

  // Combination (nCk)
  T C(int n, int k) const {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }
};

auto solve() {
  int n;
  cin >> n;

  Factorials fs(n);

  vector<vector<int>> g(n);
  vector<pair<int, int>> edges;
  REP(i, n - 1) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
    if (a > b) swap(a, b);
    edges.push_back({a, b});
  }

  auto subsize = vector(n, 0);
  auto dp = vector(n, Mint(0));

  auto dfs_size = [&](auto &rec, int v, int p) -> int {
    int ret = 1;
    for (auto u : g[v]) {
      if (u == p) continue;
      ret += rec(rec, u, v);
    }
    return subsize[v] = ret;
  };
  auto dfs_dp = [&](auto &rec, int v, int p) -> Mint {
    Mint ret = 1;
    int m = 1;
    for (auto u : g[v]) {
      if (u == p) continue;
      m += subsize[u];
      ret *= fs.ifact[subsize[u]];
    }
    ret *= fs.fact[m - 1];
    for (auto u : g[v]) {
      if (u == p) continue;
      auto sub = rec(rec, u, v);
      ret *= sub;
    }
    return dp[v] = ret;
  };

  Mint ans = 0;
  for (auto [a, b] : edges) {
    dfs_size(dfs_size, a, b);
    dfs_size(dfs_size, b, a);
    dfs_dp(dfs_dp, a, b);
    dfs_dp(dfs_dp, b, a);
    Mint res = fs.fact[n - 2];
    res *= fs.ifact[subsize[a] - 1];
    res *= fs.ifact[subsize[b] - 1];
    res *= dp[a];
    res *= dp[b];
    ans += res;
    // DUMP(a, b, subsize[a], subsize[b], dp[a], dp[b], res, ans);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
