#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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

auto solve() {
  int n;
  cin >> n;
  vector<pair<i64, i64>> lr(n);
  REP(i, n) {
    i64 l, r;
    cin >> l >> r;
    lr[i] = {l, r};
  }
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }

  vector<array<i64, 2>> beauty(n);

  auto dfs = [&](auto &dfs, int v, int p) -> array<i64, 2> {
    array<i64, 2> res = {0, 0};
    if (ssize(g[v]) == 1 and g[v].front() == p) {
      return beauty[v] = res;  // leaf
    }

    const auto [lv, rv] = lr[v];

    for (auto u : g[v]) {
      if (u == p) continue;
      const auto [xl, xr] = dfs(dfs, u, v);
      const auto [lu, ru] = lr[u];
      res[0] += max(abs(lv - lu) + xl, abs(lv - ru) + xr);
      res[1] += max(abs(rv - lu) + xl, abs(rv - ru) + xr);
    }
    return beauty[v] = res;
  };
  // REP(v, n) { DUMP(v, beauty[v][0], beauty[v][1]); }
  auto [x1, x2] = dfs(dfs, 0, -1);
  return max(x1, x2);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) cout << solve() << "\n";
}
