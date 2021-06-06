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

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

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
  atcoder::scc_graph sccg(n);
  set<pair<int, int>> edges;
  REP(i, m) {
    int x, y;
    cin >> x >> y;
    --x, --y;
    if (edges.count({x, y})) continue;
    edges.insert({x, y});
    sccg.add_edge(x, y);
  }
  auto scc = sccg.scc();
  vector<int> comp_id(n, -1);
  REP(i, scc.size()) {
    const auto &comp = scc[i];
    for (auto v : comp) {
      comp_id[v] = i;
    }
    DUMP(comp);
  }
  DUMP(comp_id);

  const int C = scc.size();
  vector<vector<int>> cg(C);
  for (auto [x, y] : edges) {
    int cx = comp_id[x];
    int cy = comp_id[y];
    if (cx != cy) {
      cg[cx].push_back(cy);
    }
  }
  vector<char> visited(C, 0);

  vector<int> cdepth(C, -1);
  auto dfs_depth = [&](auto &dfs, int v) -> void {
    for (auto u : cg[v]) {
      if (chmax(cdepth[u], cdepth[v] + 1)) {
        dfs(dfs, u);
      }
    }
  };
  REP(v, C) {
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
    if (comp_id[a] == comp_id[b]) {
      ans[i] = true;
    } else if (comp_id[a] > comp_id[b]) {
      ans[i] = false;
    } else {
      int ca = comp_id[a];
      int cb = comp_id[b];
      if (cdepth[ca] >= cdepth[cb]) {
        ans[i] = false;
      } else {
        queries[ca].push_back({cb, i});
      }
      // ans[i] = (bool)desc[ca].count(cb);
    }
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

  visited.assign(C, 0);
  vector<vector<int>> desc(C);

  vector<int> tmp;
  auto merge_desc = [&](int v, int u) -> void {
    tmp.clear();
    int i = 0;
    REP(j, desc[u].size()) {
      while (i < ssize(desc[v]) and desc[v][i] <= desc[u][j]) {
        if (tmp.empty() or tmp.back() < desc[v][i]) {
          tmp.push_back(desc[v][i]);
        }
        ++i;
      }
      if (tmp.empty() or tmp.back() < desc[u][j]) {
        tmp.push_back(desc[u][j]);
      }
    }
    while (i < ssize(desc[v])) {
      if (tmp.empty() or tmp.back() < desc[v][i]) {
        tmp.push_back(desc[v][i]);
      }
      ++i;
    }
    DUMP(desc[v], desc[u], tmp);
    swap(desc[v], tmp);
  };

  auto dfs = [&](auto &dfs, int v) -> void {
    // vector<pair<int, int>> children(cg[v].size());
    // REP(i, cg[v].size()) {
    //   int u = cg[v][i];
    //   children[i] = {pcount[u], u};
    // }
    // sort(ALL(children));
    desc[v].assign(1, v);
    for (auto u : cg[v]) {
      if (not visited[u]) {
        visited[u] = true;
        dfs(dfs, u);
      }
      //   if (--pcount[u] == 0) {
      //     if (ssize(desc[u]) > ssize(desc[v])) {
      //       swap(desc[u], desc[v]);
      //     }
      //   }
      merge_desc(v, u);
    }
    // for (auto [x, q] : queries[v]) {
    //   auto it = lower_bound(ALL(desc[v]), x);
    //   ans[q] = (it != desc[v].end()) and (*it == x);
    // }
  };

  for (int v = C - 1; v >= 0; --v) {
    if (not queries[v].empty()) {
      if (not visited[v]) {
        visited[v] = true;
        dfs(dfs, v);
      }
      for (auto [x, q] : queries[v]) {
        auto it = lower_bound(ALL(desc[v]), x);
        ans[q] = (it != desc[v].end()) and (*it == x);
      }
    }
  }

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
