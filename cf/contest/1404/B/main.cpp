#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
#define WHOLE(x) std::begin(x), std::end(x)
#define LEN(a) int((a).size())

template <class T>
bool chmax(T &a, T b) {
  if (a < b) return (a = move(b)), true;
  return false;
}
template <class T>
bool chmin(T &a, T b) {
  if (a > b) return (a = move(b)), true;
  return false;
}

template <typename T>
using V = std::vector<T>;
template <typename T>
vector<T> make_vec(size_t n, T a) {
  return vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
template <typename T>
istream &operator>>(istream &is, vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
ostream &pprint(const Container &a, string_view sep = " ",
                string_view ends = "\n", ostream *os = nullptr) {
  if (os == nullptr) os = &cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &a) {
  return pprint(a, ", ", "", &(os << "[")) << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const map<T, U> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

struct Graph {
  int n;
  V<V<int>> adj;
  explicit Graph(int n) : n(n), adj(n) {}
  void input_undirected(int m) {
    for (int i = 0; i < m; ++i) {
      int u, v;
      cin >> u >> v;
      u--;
      v--;
      adj[u].push_back(v);
      adj[v].push_back(u);
    }
  }

  void input_directed(int m) {
    for (int i = 0; i < m; ++i) {
      int u, v;
      cin >> u >> v;
      u--;
      v--;
      adj[u].push_back(v);
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

// Tree diameter.
// Returns the maximum diameter and two end point nodes with the diameter.
int tree_diameter(const V<V<int>> &adj) {
  auto dfs = [&](auto self, int v, int p, int d) -> pair<int, int> {
    int res_d = d, res_v = v;
    for (auto u : adj[v]) {
      if (u != p) {
        auto [child_d, child_v] = self(self, u, v, d + 1);
        if (child_d > res_d) {
          res_d = child_d;
          res_v = child_v;
        }
      }
    }
    return {res_d, res_v};
  };

  auto [d1, u] = dfs(dfs, 0, -1, 0);
  auto [d2, v] = dfs(dfs, u, -1, 0);
  return d2;
}

bool solve(int t) {
  int n, a, b, da, db;
  cin >> n >> a >> b >> da >> db;
  a--;
  b--;
  Graph g(n);
  g.input_undirected(n - 1);
  LCA lca(g);

  int inid = lca.distance(a, b);
  if (inid <= da) return true;

  int diam = tree_diameter(g.adj);
  DEBUG(da, db, diam);

  return min(db, diam) <= 2 * da;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) { cout << (solve(i) ? "Alice\n" : "Bob\n"); }
}
