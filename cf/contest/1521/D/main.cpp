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
  const auto e = std::end(a);
  bool first = true;
  for (auto it = std::begin(a); it != e; ++it, first = false) {
    if (!first) os << sep;
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

template <typename T,
          typename = std::enable_if_t<
              is_iterable<T>::value && !std::is_same<T, std::string>::value &&
              !std::is_same<T, std::string_view>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

struct UnionFind {
  int n;
  mutable std::vector<int> parent;  // positive: parent, negative: size
  int num_roots;

  explicit UnionFind(int sz) : n(sz), parent(sz, -1), num_roots(sz) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent[x] < -parent[y]) std::swap(x, y);
    parent[x] += parent[y];
    parent[y] = x;
    --num_roots;
    return true;
  }

  int find(int v) const {
    if (parent[v] < 0) return v;
    return parent[v] = find(parent[v]);
  }

  int size(int v) const { return -parent[find(v)]; }

  bool same(int x, int y) const { return find(x) == find(y); }

  std::vector<int> roots() const {
    std::vector<int> res;
    res.reserve(num_roots);
    for (int i = 0; i < n; ++i) {
      if (parent[i] < 0) res.push_back(i);
    }
    return res;
  }
};

struct Edge {
  int u;
  int v;
};

vector<array<int, 4>> solve() {
  int n;
  cin >> n;
  vector<set<int>> g(n);
  REP(i, n - 1) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a].insert(b);
    g[b].insert(a);
  }
  queue<pair<int, int>> q;
  vector<char> done(n, false);
  REP(v, n) {
    if (ssize(g[v]) == 1) {
      int u = *g[v].begin();
      q.push({u, v});
      done[v] = true;
    }
  }

  vector<pair<int, int>> cuts;

  while (not q.empty()) {
    auto [v, from] = q.front();
    q.pop();
    DUMP(v, from);
    if (done[v]) continue;
    if (ssize(g[v]) == 1) {
      done[v] = true;
      continue;
    }
    if (ssize(g[v]) == 2) {
      for (auto u : g[v]) {
        if (u == from) continue;
        done[v] = true;
        if (not done[u]) q.push({u, v});
        break;
      }
      continue;
    }
    assert(ssize(g[v]) >= 3);
    int path_node = -1;
    for (auto u : g[v]) {
      if (u == from) continue;
      if (done[u]) {
        path_node = u;
        break;
      }
    }
    if (path_node == -1) {
      q.push({v, from});
      DUMP("retry", v, from);
      continue;
    }

    DUMP(v, path_node);
    done[v] = true;
    vector<int> neighbors(ALL(g[v]));
    for (auto u : neighbors) {
      if (u == from or u == path_node) continue;
      cuts.push_back({min(u, v), max(u, v)});
      DUMP(cuts.back());
      g[u].erase(v);
      g[v].erase(u);
      if (ssize(g[u]) == 1) {
        int w = *g[u].begin();
        q.push({w, u});
        done[u] = true;
      }
    }
  }
  vector<int> color(n, -1);
  int color_counter = 0;
  map<int, vector<int>> ends;
  REP(v, n) {
    if (color[v] != -1) continue;
    color_counter++;
    queue<int> q;
    color[v] = color_counter;
    q.push(v);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      if (ssize(g[u]) == 1) {
        ends[color_counter].push_back(u);
      } else if (g[u].empty()) {
        ends[color_counter].push_back(u);
        ends[color_counter].push_back(u);
      }
      for (auto w : g[u]) {
        if (color[w] == -1) {
          color[w] = color_counter;
          q.push(w);
        }
      }
    }
  }

  const int m = ssize(cuts);
  assert(ssize(ends) == m + 1);
  for (const auto &[comp, e] : ends) {
    assert(ssize(e) == 2);
  }
  auto &path_ends = ends.begin()->second;
  auto it = next(ends.begin());
  vector<array<int, 4>> ans(m);
  REP(i, m) {
    ans[i][0] = cuts[i].first + 1;
    ans[i][1] = cuts[i].second + 1;
    ans[i][2] = it->second[0] + 1;
    ans[i][3] = path_ends[1] + 1;
    path_ends[1] = it->second[1];
    ++it;
  }
  assert(it == ends.end());
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    DUMP("test case", i);
    auto res = solve();
    cout << res.size() << "\n";
    REP(i, res.size()) { print_seq(res[i]); }
  }
}
