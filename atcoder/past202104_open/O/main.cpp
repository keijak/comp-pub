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
  return (int) a.size();
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

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
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct VersatileInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// Heavy-Light Decomposition
struct HLDecomp {
  // Whether the given adjacency list only contains children or may also contain
  // parents. If true, skips removing parent from the graph for speedup.
  static constexpr bool kChildrenOnlyGraph = false;

  using NodeID = int;                       // [0, n)
  using G = std::vector<std::vector<int>>;  // undirected graph
  // half-open intervals of preorder indices of nodes.
  using IntervalVec = std::vector<std::pair<int, int>>;

  int n;                       // number of nodes in the tree
  NodeID root;                 // root of the tree
  G child;                     // children node ids
  std::vector<NodeID> parent;  // parent node id (or -1 on the root node)
  std::vector<int> subsize;    // subtree size
  // "ord" is preorder index in DFS traversal. [0, n)
  std::vector<int> node_to_ord;     // node id to preorder index
  std::vector<NodeID> ord_to_node;  // preorder index to node id
  std::vector<NodeID> comp_root;    // node id to its heavy path component

  explicit HLDecomp(G g, NodeID root = 0)
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
      assert(parent[crv] != -1);
      v = parent[crv];
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
      assert(parent[crv] != -1);
      v = parent[crv];
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
      assert(parent[crv] != -1);
      v = parent[crv];
    }
    return res;
  }

  // Distance (= number of edges) of the path between two nodes.
  int distance(NodeID u, NodeID v) const {
    int dist = 0;
    for (const auto &[l, r]: edge_ranges_on_path(u, v)) {
      dist += r - l;
    }
    return dist;
  }

  // Whether the node x is on the u-v path.
  bool node_is_on_path(NodeID x, const std::pair<NodeID, NodeID> &path) const {
    const auto&[u, v] = path;
    return distance(u, x) + distance(x, v) == distance(u, v);
  }

 private:
  // Fills `parent` and `subsize`.
  void dfs_subsize(NodeID v) {
    auto &edges = child[v];
    if constexpr(not kChildrenOnlyGraph) {
      if (parent[v] != -1) {
        // Remove the parent from `child[v]`. Amortized O(N).
        auto it = std::find(edges.begin(), edges.end(), parent[v]);
        if (it != edges.end()) {
          std::swap(*it, edges.back());
          edges.pop_back();
        }
      }
    }
    for (NodeID &u: edges) {
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
    for (NodeID u: child[v]) {
      comp_root[u] = (u == child[v][0] ? comp_root[v] : u);
      dfs_hld(u, counter);
    }
  }
};

// Returns the cumulative sum array of edge costs (indexed by Ord).
std::vector<i64> edge_cost_cumsum(
    const HLDecomp &hld, const std::map<std::pair<int, int>, i64> &edge_cost) {
  const int n = hld.n;
  std::vector<i64> cost(n), acc(n + 1);  // indexed by Ord.
  // Iterate over the NodeID space.
  for (int v = 0; v < n; ++v) {
    // Calculate the cost from v to parent[v].
    auto p = hld.parent[v];
    if (p == -1) continue;
    auto it = edge_cost.find(std::pair{std::min(v, p), std::max(v, p)});
    if (it == edge_cost.end()) continue;
    const i64 cost_to_parent = it->second;
    const int ord = hld.node_to_ord[v];
    cost[ord] = cost_to_parent;
  }
  // Iterate over the Ord space.
  for (int i = 0; i < n; ++i) {
    acc[i + 1] = acc[i] + cost[i];
  }
  return acc;
}

const i64 kBig = 1e16;

template<class T>
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

// Returns min distance from the start node to each node (if exists).
auto search_shortest_path(const vector<vector<Edge>> &g, int start, int goal) {
  const int n = g.size();
  auto mincost = vector(n, kBig);
  MinHeap<State> que;
  auto push = [&](i64 cost, int node) -> bool {
    if (chmin(mincost[node], cost)) {
      que.push(State{cost, node});
      return true;
    }
    return false;
  };
  assert(push(0LL, start));

  while (not que.empty()) {
    State cur = que.top();
    que.pop();
    if (cur.cost != mincost[cur.node]) continue;
    if (cur.node == goal) break;
    for (const auto &e: g[cur.node]) {
      auto new_cost = cur.cost + e.cost;
      push(new_cost, e.to);
    }
  }
  return mincost[goal];
}

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  int index(T x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  const T &value(int i) const { return values[i]; }

  bool contains(T x) const {
    return std::binary_search(values.begin(), values.end(), x);
  }
};

struct DFSTree {
  int n_;
  int root_;
  vector<vector<int>> g_;  // a spanning tree of the original graph.
  vector<int> parent_;
  vector<int> depth_;

  // edge(u,v) where v is an ancestor of u in the DFS tree.
  set<pair<int, int>> back_edges_;

  explicit DFSTree(vector<vector<int>> g, int root = 0)
      : n_((int) g.size()), root_(root), g_(g), parent_(n_, -1), depth_(n_, -1) {
    dfs(root_, -1, 0);
    for (int v = 0; v < n_; ++v) {
      std::sort(g[v].begin(), g[v].end());
    }
  }

  bool is_connected(int u, int v) const {
    if (depth_[u] < depth_[v]) std::swap(u, v);
    return (parent_[u] == v) or back_edges_.count({u, v});
  }

 private:
  void dfs(int v, int p, int d) {
    parent_[v] = p;
    depth_[v] = d;
    set<int> non_neighbors;
    for (auto u: g_[v]) {
      if (u == p) continue;
      if (depth_[u] == -1) {
        dfs(u, v, d + 1);
        continue;
      }
      non_neighbors.insert(u);
      if (depth_[u] < d) {
        // backward
        back_edges_.emplace(v, u);
      } else {
        // forward
      }
    }
    g_[v].erase(std::remove_if(g_[v].begin(), g_[v].end(),
                               [&](int u) { return non_neighbors.count(u); }),
                g_[v].end());
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n = in, m = in;
  vector<vector<int>> g(n);
  REP(i, m) {
    auto u = int(in) - 1, v = int(in) - 1;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  DFSTree tree(g);
  HLDecomp hld(tree.g_);

  vector<int> spnodes;
  for (auto[u, v]: tree.back_edges_) {
    spnodes.push_back(u);
    spnodes.push_back(v);
  }
  Compressed<int> csp(spnodes);

  const int Q = in;
  REP(qi, Q) {
    vector<vector<Edge>> qg(csp.size());
    for (auto[x, y]: tree.back_edges_) {
      int xi = csp.index(x);
      int yi = csp.index(y);
      qg[xi].push_back(Edge{1, yi});
      qg[yi].push_back(Edge{1, xi});
    }
    REP(i, csp.size()) {
      REP(j, csp.size()) {
        if (i == j) continue;
        int x = csp.value(i);
        int y = csp.value(j);
        if (tree.depth_[x] < tree.depth_[y]) swap(x, y);
        if (tree.back_edges_.count({x, y})) continue;
        i64 d = hld.distance(x, y);
        qg[i].push_back(Edge{d, j});
        qg[j].push_back(Edge{d, i});
      }
    }

    auto u = int(in) - 1, v = int(in) - 1;
    bool outside = (not csp.contains(u)) and (not csp.contains(v));
    int ui, vi;
    if (csp.contains(u)) {
      ui = csp.index(u);
    } else {
      ui = ssize(qg);
      vector<Edge> adj;
      REP(i, csp.size()) {
        int x = csp.value(i);
        int d = hld.distance(u, x);
        adj.push_back(Edge{d, i});
        qg[i].push_back(Edge{d, ui});
      }
      qg.push_back(move(adj));
    }
    if (csp.contains(v)) {
      vi = csp.index(v);
    } else {
      vi = ssize(qg);
      vector<Edge> adj;
      REP(i, csp.size()) {
        int x = csp.value(i);
        int d = hld.distance(v, x);
        adj.push_back(Edge{d, i});
        qg[i].push_back(Edge{d, vi});
      }
      qg.push_back(move(adj));
    }

    if (outside) {
      i64 d = hld.distance(u, v);
      qg[ui].push_back(Edge{d, vi});
      qg[vi].push_back(Edge{d, ui});
    }

    auto res = search_shortest_path(qg, ui, vi);
    print(res);
  }
}
