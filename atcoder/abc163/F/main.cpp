#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

i64 num_paths(i64 n) { return n * (n + 1) / 2; }

void solve() {
  int n;
  cin >> n;
  vector<int> C(n);
  REP(i, n) {
    cin >> C[i];
    --C[i];
  }
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  vector<int> subsize(n, 0);
  auto dfs_subsize = [&](auto &rec, int v, int p) -> int {
    int res = 1;
    for (auto u : g[v]) {
      if (u == p) continue;
      res += rec(rec, u, v);
    }
    subsize[v] = res;
    return res;
  };
  dfs_subsize(dfs_subsize, 0, -1);
  DUMP(subsize);

  vector<i64> path_counts(n, 0LL);
  vector<int> c_ancestor(n, -1);
  vector<i64> removed(n, 0LL);
  vector<i64> root_removed(n, 0LL);
  auto dfs_solve = [&](auto &rec, int v, int p) -> void {
    int my_color = C[v];
    int c_ancestor_old = c_ancestor[my_color];
    for (auto u : g[v]) {
      if (u == p) continue;
      c_ancestor[my_color] = u;
      rec(rec, u, v);
      i64 component_size = subsize[u] - removed[u];
      path_counts[my_color] += num_paths(component_size);
    }
    c_ancestor[my_color] = c_ancestor_old;
    if (c_ancestor_old == -1) {
      root_removed[my_color] += subsize[v];
    } else {
      removed[c_ancestor_old] += subsize[v];
    }
  };
  dfs_solve(dfs_solve, 0, -1);
  REP(c, n) {
    i64 root_path_counts = num_paths(n - root_removed[c]);
    i64 ans = num_paths(n) - path_counts[c] - root_path_counts;
    cout << ans << "\n";
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
