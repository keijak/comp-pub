#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
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
    if (it != b) os << sep;
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
void print(const T &x) {
  std::cout << x << "\n";
}
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
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// LowLink
// Computes bridges and articulation points of a graph in O(V+E).
struct LowLink {
  using G = std::vector<std::vector<int>>;

  int n;
  G adj;
  std::vector<int> ord, low;
  std::vector<int> aps;  // articulation points
  std::vector<std::pair<int, int>> bridges;

  explicit LowLink(G g) : n(g.size()), adj(move(g)), ord(n, -1), low(n) {
    init();
  }

 private:
  void init() {
    int ord_counter = 0;

    auto dfs = [&](auto self, int v, int par = -1) -> void {
      ord[v] = ord_counter++;
      low[v] = ord[v];
      bool is_articulation_point = false;
      for (auto u : adj[v]) {
        if (ord[u] < 0) {
          self(self, u, v);
          low[v] = std::min(low[v], low[u]);
          if (par >= 0 and ord[v] <= low[u]) {
            is_articulation_point = true;
          }
          if (ord[v] < low[u]) {
            bridges.emplace_back(std::min(u, v), std::max(u, v));
          }
        } else if (u != par) {  // backward edge
          low[v] = std::min(low[v], ord[u]);
        }
      }
      // Is the root node an articulatin point?
      if (par < 0 and adj[v].size() >= 2) {
        is_articulation_point = true;
      }
      if (is_articulation_point) aps.push_back(v);
    };

    for (int i = 0; i < n; ++i) {
      if (ord[i] == -1) dfs(dfs, i);
    }

    // sort(aps.begin(), aps.end());
    // sort(bridges.begin(), bridges.end());
  }
};

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

template<typename Task>
class Rerooting {
 private:
  using NV = typename Task::NodeValue;
  using EV = typename Task::EdgeValue;

  Task task;
  int n;                             // number of nodes
  std::vector<std::vector<int>> g;  // graph (tree)
  std::vector<NV> sub;               // values for each subtree rooted at i
  std::vector<NV> full;              // values for each entire tree rooted at i
  int base_root;                     // base root node where we start DFS

 public:
  explicit Rerooting(Task task, std::vector<std::vector<int>> g, int r = 0)
      : task(move(task)),
        n((int) g.size()),
        g(move(g)),
        sub(n),
        full(n),
        base_root(r) {}

  const std::vector<NV> &run() {
    pull_up(base_root, -1);
    push_down(base_root, -1, std::nullopt);
    return full;
  }

 private:
  NV pull_up(int v, int par) {
    EV res = task.id();
    for (auto &e : g[v]) {
      int u = e;
      if (u == par) continue;
      auto sub = pull_up(u, v);
      res = task.merge(res, sub);
    }
    return (sub[v] = task.add_node(res, v));
  }

  void push_down(int v, int par, std::optional<NV> upper_sub) {
    int m = g[v].size();
    std::vector<EV> cuml(m + 1, task.id()), cumr(m + 1, task.id());

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e;
      if (u == par) {
        assert(upper_sub.has_value());
        cuml[i + 1] = task.merge(cuml[i], *upper_sub);
      } else {
        cuml[i + 1] = task.merge(cuml[i], sub[u]);
      }
    }

    for (int i = m - 1; i >= 0; --i) {
      auto &e = g[v][i];
      int u = e;
      if (u == par) {
        cumr[i] = task.merge(*upper_sub, cumr[i + 1]);
      } else {
        cumr[i] = task.merge(sub[u], cumr[i + 1]);
      }
    }

    full[v] = task.add_node(cuml[m], v);

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e;
      if (u == par) continue;
      std::optional<NV> next_upper_sub{
          task.add_node(task.merge(cuml[i], cumr[i + 1]), v)};
      push_down(u, v, std::move(next_upper_sub));
    }
  }
};

struct Task {
  using NodeValue = i64;
  using EdgeValue = i64;
  vector<i64> w;

  explicit Task(vector<i64> rw) : w(move(rw)) {}

  EdgeValue id() const { return 0; }

  NodeValue add_node(EdgeValue val, int node) const {
    return val + w[node];
  }

  EdgeValue merge(EdgeValue v1, EdgeValue v2) const {
    return max(v1, v2);
  }
};

auto solve() {
  INPUT(int, n, m);
  vector<int> w(n);
  cin >> w;
  vector<vector<int>> g(n);
  REP(i, m) {
    INPUT(int, u, v);
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }

  i64 ans = w[0];
  vector<int> visited(n, 0);
  auto dfs = [&](auto &dfs, int v, int p, i64 score) -> i64 {
    for (auto u : g[v]) {
      if (u == p) continue;
      if (visited[u] >= 2) continue;
      i64 new_score = score;
      if (visited[u] == 0) {
        new_score += w[u];
        chmax(ans, new_score);
      }
      visited[u]++;
      dfs(dfs, u, v, new_score);

    }

  };
  dfs(dfs, 0, -1, w[0]);

  LowLink low_link(g);
  DUMP(low_link.bridges);

  vector<set<int>> g2(n);
  REP(i, n) {
    g2[i] = set<int>(ALL(g[i]));
  }
  for (auto[u, v] : low_link.bridges) {
    g2[u].erase(v);
    g2[v].erase(u);
  }

  UnionFind uf(n);
  REP(v, n) {
    for (auto u : g2[v]) {
      uf.unite(u, v);
    }
  }

  int p = uf.num_roots;
  map<int, int> root_id;
  vector<int> roots;
  REP(v, n) {
    if (uf.find(v) == v) {
      int rid = roots.size();
      roots.push_back(v);
      root_id[v] = rid;
    }
  }
  vector<i64> rw(p);
  REP(v, n) {
    int j = root_id[uf.find(v)];
    rw[j] += w[v];
  }
  vector<vector<int>> g3(p);
  for (auto[u, v] : low_link.bridges) {
    int ru = root_id[uf.find(u)];
    int rv = root_id[uf.find(v)];
    g3[ru].push_back(rv);
    g3[rv].push_back(ru);
  }

  REP(r, p) {
    DUMP(r, rw[r], g3[r]);
  }

  Task task(rw);
  Rerooting<Task> rr(task, g3);
  auto res = rr.run();

  i64 ans = 0;
  REP(r, p) {
    DUMP(r, res[r]);
    chmax(ans, res[r]);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
