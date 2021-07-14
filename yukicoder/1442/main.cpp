#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
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
void pdebug(const T &value, const Ts &...args) {
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

const i64 BIG = 1e16;

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  i64 cost;
  int to;
};

struct State {
  i64 cost;
  int node;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the source node to each node (if exists).
auto search(const vector<vector<Edge>> &g, int source) {
  const int n = g.size();
  vector mincost(n, BIG);
  MinHeap<State> que;
  auto push = [&](i64 cost, int node) -> bool {
    if (chmin(mincost[node], cost)) {
      que.push({cost, node});
      return true;
    }
    return false;
  };
  assert(push(0LL, source));

  while (not que.empty()) {
    State cur = move(que.top());
    que.pop();
    if (cur.cost != mincost[cur.node]) {
      continue;
    }
    for (const auto &e : g[cur.node]) {
      i64 new_cost = cur.cost + e.cost;
      push(new_cost, e.to);
    }
  }
  return mincost;
}

// Heavy-Light Decomposition
struct HLD {
  using NodeID = int;                       // [0, n)
  using G = std::vector<std::vector<int>>;  // undirected graph
  // half-open intervals of preorder indices of nodes.
  using IntervalVec = std::vector<std::pair<int, int>>;

  int n;                                      // number of nodes in the tree
  NodeID root;                                // root of the tree
  G child;                                    // children node ids
  std::vector<std::optional<NodeID>> parent;  // parent node id (or -1)
  std::vector<int> subsize;                   // subtree size
  // "ord" is preorder index in DFS traversal. [0, n)
  std::vector<int> node_to_ord;     // node id to preorder index
  std::vector<NodeID> ord_to_node;  // preorder index to node id
  std::vector<NodeID> comp_root;    // node id to its heavy path component

  explicit HLD(G g, NodeID root = 0)
      : n(int(g.size())),
        root(root),
        child(g),
        parent(n, -1),
        subsize(n, 1),
        node_to_ord(n, -1),
        ord_to_node(n, -1),
        comp_root(n, -1) {
    dfs_subsize(root);
    int counter = 0;
    comp_root[root] = root;
    dfs_hld(root, counter);
  }

  // Lowest Common Ancestor
  NodeID lca(NodeID u, NodeID v) const {
    for (;;) {
      if (node_to_ord[u] > node_to_ord[v]) std::swap(u, v);
      NodeID crv = comp_root[v];
      if (comp_root[u] == crv) return u;
      assert(parent[crv].has_value());
      v = parent[crv].value();
    }
  }

  // Returns the set of nodes on the u-v path, including both u and v.
  //
  // The return value is half-open intervals of the preorder indices of the
  // nodes. One interval corresponds to one heavy path component.
  IntervalVec node_ranges_on_path(NodeID u, NodeID v) const {
    IntervalVec res;
    for (;;) {
      if (node_to_ord[u] > node_to_ord[v]) std::swap(u, v);
      NodeID crv = comp_root[v];
      res.emplace_back(std::max(node_to_ord[crv], node_to_ord[u]),
                       node_to_ord[v] + 1);
      if (comp_root[u] == crv) break;
      assert(parent[crv].has_value());
      v = parent[crv].value();
    }
    return res;
  }

  // Returns the set of edges in the u-v path.
  //
  // The return value is half-open intervals of the preorder indices of nodes
  // corresponding to the deeper end (closer to leaves) of each edge in the
  // path. Here we identify Edge(v, parent[v]) as v.
  IntervalVec edge_ranges_on_path(NodeID u, NodeID v) const {
    IntervalVec res;
    for (;;) {
      if (node_to_ord[u] > node_to_ord[v]) std::swap(u, v);
      NodeID crv = comp_root[v];
      if (comp_root[u] == crv) {
        if (u != v) res.emplace_back(node_to_ord[u] + 1, node_to_ord[v] + 1);
        break;
      }
      res.emplace_back(node_to_ord[crv], node_to_ord[v] + 1);
      assert(parent[crv].has_value());
      v = parent[crv].value();
    }
    return res;
  }

  // Distance (= number of edges) of the path between two nodes.
  int distance(NodeID u, NodeID v) const {
    int dist = 0;
    for (auto [l, r] : edge_ranges_on_path(u, v)) {
      dist += r - l;
    }
    return dist;
  }

 private:
  // Fills `parent` and `subsize` and drops parent node ids from `child`.
  void dfs_subsize(NodeID v) {
    auto &edges = child[v];
    if (parent[v].has_value()) {
      auto it = std::find(edges.begin(), edges.end(), parent[v].value());
      if (it != edges.end()) edges.erase(it);
    }
    for (NodeID &u : edges) {
      parent[u] = v;
      dfs_subsize(u);
      subsize[v] += subsize[u];
      if (subsize[u] > subsize[edges[0]]) {
        std::swap(u, edges[0]);
      }
    }
  }

  // Fills `node_to_ord`, `ord_to_node`, and `comp_root`.
  void dfs_hld(NodeID v, int &counter) {
    node_to_ord[v] = counter++;
    ord_to_node[node_to_ord[v]] = v;
    for (NodeID u : child[v]) {
      comp_root[u] = (u == child[v][0] ? comp_root[v] : u);
      dfs_hld(u, counter);
    }
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n, k;
  cin >> n >> k;
  vector<vector<Edge>> g(n + k);
  vector<vector<int>> g2(n);
  map<pair<int, int>, i64> edge_to_cost;
  REP(i, n - 1) {
    int a, b;
    i64 c;
    cin >> a >> b >> c;
    --a, --b;
    g[a].push_back({c, b});
    g[b].push_back({c, a});
    g2[a].push_back(b);
    g2[b].push_back(a);
    edge_to_cost[pair{min(a, b), max(a, b)}] = c;
  }

  vector<i64> flight_cost(k);
  vector<vector<i64>> mincost_table(k);
  REP(i, k) {
    i64 m, p;
    cin >> m >> p;
    flight_cost[i] = p;
    int sup = n + i;
    REP(j, m) {
      int x;
      cin >> x;
      --x;
      g[sup].push_back({0, x});
      g[x].push_back({p, sup});
    }
  }
  REP(i, k) {
    int sup = n + i;
    i64 p = flight_cost[i];
    mincost_table[i] = search(g, sup);
  }
  HLD hld(g2);
  vector<i64> base_cost(n), acc(n + 1, 0LL);
  REP(i, n) {
    auto p = hld.parent[i];
    if (not p.has_value()) continue;
    i64 c = edge_to_cost[pair{min(i, *p), max(i, *p)}];
    int j = hld.node_to_ord[i];
    base_cost[j] = c;
  }
  REP(i, n) acc[i + 1] = acc[i] + base_cost[i];

  int q;
  cin >> q;
  REP(qi, q) {
    int u, v;
    cin >> u >> v;
    --u, --v;

    i64 ans = BIG;
    REP(i, k) {
      i64 cu = mincost_table[i][u];
      i64 cv = mincost_table[i][v];
      chmin(ans, cu + cv + flight_cost[i]);
    }
    {
      i64 cost = 0;
      for (auto [l, r] : hld.edge_ranges_on_path(u, v)) {
        cost += acc[r] - acc[l];
      }
      chmin(ans, cost);
    }
    cout << ans << "\n";
  }
}
