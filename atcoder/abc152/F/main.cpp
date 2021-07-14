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

#ifdef MY_DEBUG
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
  map<pair<int, int>, int> edge_id;
  V<pair<int, int>> edge;

  explicit Graph(int n) : n(n), adj(n) {}
  void input_undirected(int m) { input(m, false); }
  void input_directed(int m) { input(m, true); }

 private:
  void input(int m, bool is_directed) {
    edge.resize(m);
    for (int i = 0; i < m; ++i) {
      int u, v;  // read as 1-indexed.
      cin >> u >> v;
      u--, v--;  // to 0-indexed.
      edge[i] = {u, v};
      edge_id[{u, v}] = edge_id[{v, u}] = i;
      adj[u].push_back(v);
      if (not is_directed) adj[v].push_back(u);
    }
  }
};

// Lowest Common Ancestor of a tree.
class LCA {
 public:
  const int n;  // number of nodes
  const vector<vector<int>> &adj;
  vector<vector<int>> upper;  // 2^k upper node
  vector<int> depth;

  explicit LCA(const Graph &g)
      : n(g.n), adj(g.adj), upper(K, vector<int>(g.n, -1)), depth(g.n, -1) {
    dfs(0, -1, 0);
    for (int k = 0; k + 1 < K; k++) {
      REP(v, n) {
        if (upper[k][v] < 0) {
          upper[k + 1][v] = -1;
        } else {
          upper[k + 1][v] = upper[k][upper[k][v]];
        }
      }
    }
  }

  // Returns the node ID of the lowest common ancestor.
  int ancestor(int u, int v) const {
    if (depth[u] > depth[v]) swap(u, v);
    for (int k = 0; k < K; k++) {
      if ((depth[v] - depth[u]) & (1 << k)) {
        v = upper[k][v];
      }
    }
    if (u == v) return u;
    for (int k = K - 1; k >= 0; k--) {
      if (upper[k][u] != upper[k][v]) {
        u = upper[k][u];
        v = upper[k][v];
      }
    }
    return upper[0][u];
  }

  int parent(int v) const { return upper[0][v]; }

  // Returns the distance (number of edges) between two nodes.
  int distance(int u, int v) const {
    int p = ancestor(u, v);
    return (depth[u] - depth[p]) + (depth[v] - depth[p]);
  }

 private:
  void dfs(int v, int p, int d) {
    upper[0][v] = p;
    depth[v] = d;
    for (auto x : adj[v]) {
      if (x == p) continue;
      dfs(x, v, d + 1);
    }
  }

  static const int K = 30;  // max upper lookup (2^K)
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;

  Graph g(N);
  g.input_undirected(N - 1);
  LCA lca(g);

  int M;
  cin >> M;
  V<u64> constraints(M);
  REP(i, M) {
    int u, v;
    cin >> u >> v;
    u--;
    v--;

    u64 cons = 0LL;
    int a = lca.ancestor(u, v);
    for (int c = u, p = lca.parent(c); c != a; c = p, p = lca.parent(p)) {
      int e = g.edge_id[{c, p}];
      cons |= 1ULL << e;
    }
    for (int c = v, p = lca.parent(c); c != a; c = p, p = lca.parent(p)) {
      int e = g.edge_id[{c, p}];
      cons |= 1ULL << e;
    }
    constraints[i] = cons;
  }

  i64 ans = 1ULL << (N - 1);
  for (int x = 1; x <= M; ++x) {
    V<int> p(M, 0);
    REP(i, x) p[M - 1 - i] = 1;
    do {
      u64 constraint = 0LL;
      REP(i, M) {
        if (p[i] == 1) {
          constraint |= constraints[i];
        }
      }
      int changeable = 0;
      REP(i, N - 1) {
        if (not(constraint >> i & 1)) ++changeable;
      }
      if (x & 1) {
        ans -= (1ULL << changeable);
      } else {
        ans += (1ULL << changeable);
      }
    } while (next_permutation(ALL(p)));
  }
  cout << ans << endl;
}
