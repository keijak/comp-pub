#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...)                                             \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) { return (int) std::size(a); }

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a)
    is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template<typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b)
      os << sep;
    os << *it;
  }
  return os << ends;
}
template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) { std::cout << x << "\n"; }
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...)                                                       \
  type __VA_ARGS__;                                                            \
  read_from_cin(__VA_ARGS__);

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

auto solve() -> void {
  INPUT(int, n);
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    INPUT(int, u, v);
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }

  auto sizes = vector(n, 0);
  auto dfs_sizes = [&](auto &rec, int v, int p) -> int {
    int sz = 1;
    for (auto u : g[v]) {
      if (u == p)continue;
      sz += rec(rec, u, v);
    }
    sizes[v] = sz;
    return sz;
  };
  dfs_sizes(dfs_sizes, 0, -1);
  DUMP(sizes);

  auto dp = vector(n, vector(3, vector<Mint>()));

  auto dfs = [&](auto &dfs, int v, int p) -> void {
    int nv = 1;
    auto tmp = vector(3, vector<Mint>(sizes[v] + 1));
    REP(j, 3) dp[v][j].resize(sizes[v] + 1);
    dp[v][0][0] = 1;
    dp[v][2][1] = 1;
    for (auto u : g[v]) {
      if (u == p) continue;
      dfs(dfs, u, v);
      const int nv2 = nv + sizes[u];
      REP(i, nv + 1) {
        REP(j, sizes[u] + 1) {
          if (i + j <= nv2) tmp[0][i + j] += dp[v][0][i] * dp[u][0][j];
          if (i + j <= nv2) tmp[0][i + j] += dp[v][0][i] * dp[u][1][j];
          if (i + j + 1 <= nv2) tmp[1][i + j + 1] += dp[v][0][i] * dp[u][2][j];
          if (i + j <= nv2) tmp[1][i + j] += dp[v][1][i] * dp[u][0][j];
          if (i + j <= nv2) tmp[1][i + j] += dp[v][1][i] * dp[u][1][j];
          if (i + j <= nv2) tmp[1][i + j] += dp[v][1][i] * dp[u][2][j];
          if (i + j + 1 <= nv2) tmp[2][i + j + 1] += dp[v][2][i] * dp[u][0][j];
          if (i + j <= nv2) tmp[2][i + j] += dp[v][2][i] * dp[u][1][j];
          if (i + j <= nv2) tmp[2][i + j] += dp[v][2][i] * dp[u][2][j];
        }
      }
      swap(dp[v], tmp);
      REP(j, 3) REP(k, nv + 1) tmp[j][k] = 0;
      nv = nv2;
    }
  };
  dfs(dfs, 0, -1);

  vector<Mint> ans(n + 1);
  REP(i, 3) {
    REP(j, n + 1) { ans[j] += dp[0][i][j]; }
  }
  REP(j, n + 1) print(ans[j]);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) { solve(); }
}
