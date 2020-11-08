#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;
template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &... args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;

struct Graph {
  int n;
  V<V<int>> adj;
  std::map<pair<int, int>, int> edge_id;

  explicit Graph(int n) : n(n), adj(n), edge_id() {}

  void input_undirected(int m) {
    for (int i = 0; i < m; ++i) {
      int u, v;  // read as 1-indexed.
      cin >> u >> v;
      u--, v--;  // to 0-indexed.
      adj[u].push_back(v);
      adj[v].push_back(u);
      edge_id[{u, v}] = edge_id[{v, u}] = i;
    }
  }
};

// Lowest Common Ancestor of a tree.
class LCA {
 public:
  const int n;  // number of nodes
  const vector<vector<int>> &adj;
  vector<vector<int>> parent;
  vector<int> depth;

  explicit LCA(const Graph &g)
      : n(g.n), adj(g.adj), parent(K, vector<int>(g.n, -1)), depth(g.n, -1) {
    dfs(0, -1, 0);
    for (int k = 0; k + 1 < K; k++) {
      REP(v, n) {
        if (parent[k][v] < 0) {
          parent[k + 1][v] = -1;
        } else {
          parent[k + 1][v] = parent[k][parent[k][v]];
        }
      }
    }
  }

  // Returns the node ID of the lowest common ancestor.
  int query(int u, int v) const {
    if (depth[u] > depth[v]) swap(u, v);
    for (int k = 0; k < K; k++) {
      if ((depth[v] - depth[u]) & (1 << k)) {
        v = parent[k][v];
      }
    }
    if (u == v) return u;
    for (int k = K - 1; k >= 0; k--) {
      if (parent[k][u] != parent[k][v]) {
        u = parent[k][u];
        v = parent[k][v];
      }
    }
    return parent[0][u];
  }

  // Returns the distance (number of edges) between two nodes.
  int distance(int u, int v) const {
    int p = query(u, v);
    return (depth[u] - depth[p]) + (depth[v] - depth[p]);
  }

 private:
  void dfs(int v, int p, int d) {
    parent[0][v] = p;
    depth[v] = d;
    for (auto x : adj[v]) {
      if (x == p) continue;
      dfs(x, v, d + 1);
    }
  }

  static const int K = 30;  // max parent lookup (2^K)
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  Graph g(N);
  g.input_undirected(N - 1);
  LCA lca(g);

  V<V<tuple<int, bool, int>>> tasks(N);  // (time, add/remove, color)
  REP(i, N) { tasks[i].emplace_back(-1, true, 0); }  // init.

  REP(i, Q) {
    int u, v, c;
    cin >> u >> v >> c;
    u--;
    v--;

    int ancestor = lca.query(u, v);
    tasks[u].emplace_back(i, true, c);
    tasks[v].emplace_back(i, true, c);
    tasks[ancestor].emplace_back(i, false, c);
    // tasks[ancestor].emplace_back(i, false, c);
  }

  V<int> ans(N - 1);
  auto dfs = [&](auto self, int v, int p, int dep) -> set<tuple<int, int>> {
    set<tuple<int, int>> res;
    for (auto u : g.adj[v]) {
      if (u == p) continue;
      auto r = self(self, u, v, dep + 1);
      if (res.size() > r.size()) {
        res.insert(ALL(r));
      } else {
        r.insert(ALL(res));
        std::swap(r, res);
      }
    }
    for (auto [t, paint, color] : tasks[v]) {
      if (paint) {
        res.emplace(t, color);
      } else {
        auto it = res.find({t, color});
        res.erase(it);
      }
    }
    assert(not res.empty());
    auto [last_time, last_color] = *res.rbegin();

    if (p >= 0) {
      int e = g.edge_id[{v, p}];
      ans[e] = last_color;
    }
    return res;
  };
  dfs(dfs, 0, -1, 0);
  pprint(ans, "\n");
}
