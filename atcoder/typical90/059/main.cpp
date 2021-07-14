#include <bits/stdc++.h>

#include <atcoder/scc>

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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

#include <boost/dynamic_bitset.hpp>
using BitSet = boost::dynamic_bitset<>;

#include <boost/multiprecision/cpp_int.hpp>
using u128 = boost::multiprecision::uint128_t;
using Int = boost::multiprecision::cpp_int;

// s |= t;
// Merges two sets into their union.
// The first argument will be updated to the result.
template <typename T>
std::set<T> &operator|=(std::set<T> &s, const std::set<T> &t) {
  s.insert(t.begin(), t.end());
  return s;
}
template <typename T>
std::set<T> &operator|=(std::set<T> &s, std::set<T> &&t) {
  if (s.size() >= t.size()) {
    s.merge(t);
  } else {
    t.merge(s);
    s.swap(t);
  }
  return s;
}

void solve() {
  int n, m, Q;
  cin >> n >> m >> Q;
  set<pair<int, int>> edges;
  vector<vector<int>> g(n);
  REP(i, m) {
    int x, y;
    cin >> x >> y;
    --x, --y;
    if (edges.count({x, y})) continue;
    edges.insert({x, y});
    g[x].push_back(y);
  }

  vector<char> visited(n, 0);

  vector<int> cdepth(n, -1);
  auto dfs_depth = [&](auto &dfs, int v) -> void {
    for (auto u : g[v]) {
      if (chmax(cdepth[u], cdepth[v] + 1)) {
        dfs(dfs, u);
      }
    }
  };
  REP(v, n) {
    if (cdepth[v] < 0) {
      cdepth[v] = 0;
      dfs_depth(dfs_depth, v);
    }
  }
  DUMP(cdepth);

  vector<vector<pair<int, int>>> queries(n);
  vector<optional<bool>> ans(Q);
  REP(i, Q) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    assert(a < b);
    // if (cdepth[a] >= cdepth[b]) {
    //   ans[i] = false;
    // } else {
    queries[a].push_back({b, i});
    // ans[i] = (bool)desc[a].count(b);
    // }
  }

  //   visited.assign(C, 0);
  //   vector<int> pcount(C, 0);
  //   auto dfs_pc = [&](auto &dfs, int v) -> void {
  //     for (auto u : cg[v]) {
  //       pcount[u]++;
  //       if (not visited[u]) {
  //         visited[u] = true;
  //         dfs(dfs, u);
  //       }
  //     }
  //   };
  //   REP(v, C) {
  //     if (not visited[v]) {
  //       visited[v] = true;
  //       dfs_pc(dfs_pc, v);
  //     }
  //   }
  // DUMP(pcount);

  visited.assign(n, 0);
  vector<Int> desc(n);

  //   vector<int> tmp;
  //   auto merge_desc = [&](int v, int u) -> void {
  //     tmp.clear();
  //     int i = 0;
  //     REP(j, desc[u].size()) {
  //       while (i < ssize(desc[v]) and desc[v][i] <= desc[u][j]) {
  //         if (tmp.empty() or tmp.back() < desc[v][i]) {
  //           tmp.push_back(desc[v][i]);
  //         }
  //         ++i;
  //       }
  //       if (tmp.empty() or tmp.back() < desc[u][j]) {
  //         tmp.push_back(desc[u][j]);
  //       }
  //     }
  //     while (i < ssize(desc[v])) {
  //       if (tmp.empty() or tmp.back() < desc[v][i]) {
  //         tmp.push_back(desc[v][i]);
  //       }
  //       ++i;
  //     }
  //     DUMP(desc[v], desc[u], tmp);
  //     swap(desc[v], tmp);
  //   };

  auto dfs = [&](auto &dfs, int v) -> void {
    // vector<pair<int, int>> children(cg[v].size());
    // REP(i, cg[v].size()) {
    //   int u = cg[v][i];
    //   children[i] = {pcount[u], u};
    // }
    // sort(ALL(children));

    // desc[v] |= Int(1) << v;

    for (auto u : g[v]) {
      if (not visited[u]) {
        visited[u] = true;
        dfs(dfs, u);
      }
      //   if (--pcount[u] == 0) {
      //     if (ssize(desc[u]) > ssize(desc[v])) {
      //       swap(desc[u], desc[v]);
      //     }
      //   }
      desc[v] |= (desc[u] | (Int(1) << u));
    }
    // for (auto [x, q] : queries[v]) {
    //   auto it = lower_bound(ALL(desc[v]), x);
    //   ans[q] = (it != desc[v].end()) and (*it == x);
    // }
  };

  for (int v = n - 1; v >= 0; --v) {
    if (not visited[v]) {
      visited[v] = true;
      dfs(dfs, v);
    }
    for (auto [x, q] : queries[v]) {
      ans[q] = (bool)(desc[v] >> x & 1);
      //   auto it = lower_bound(ALL(desc[v]), x);
      //   ans[q] = (it != desc[v].end()) and (*it == x);
    }
  }

  // REP(i, n) { DUMP(i, desc[i]); }

  REP(i, Q) {
    if (ans[i].has_value()) {
      cout << (*ans[i] ? "Yes" : "No") << "\n";
    } else {
      cout << "?\n";
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
