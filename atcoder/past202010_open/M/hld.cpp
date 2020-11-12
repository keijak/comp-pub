#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)

using namespace std;

struct Graph {
  int n;
  vector<vector<int>> adj;
  map<pair<int, int>, int> edge_id;

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

// Heavy-Light Decomposition
struct HLD {
  using NodeID = int;  // [0, n)
  using G = std::vector<std::vector<int>>;

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
  NodeID lca(NodeID u, NodeID v) {
    for (;;) {
      if (node_to_ord[u] > node_to_ord[v]) std::swap(u, v);
      NodeID crv = comp_root[v];
      if (comp_root[u] == crv) return u;
      assert(parent[crv].has_value());
      v = parent[crv].value();
    }
  }

  // Returns the set of edges in the path in the form of [l, r) intervals of the
  // preorder indices of nodes corresponding to the deeper end (closer to
  // leaves) of each edge in the u-v path.
  auto edge_ranges_on_path(NodeID u, NodeID v) {
    std::vector<std::pair<int, int>> res;
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

template <typename Monoid>
struct DualSegTree {
  using T = typename Monoid::T;

  explicit DualSegTree(int n) : size_(n), data_(2 * n, Monoid::id()) {}

  inline int size() const { return size_; }

  // Returns i-th value.
  T operator[](int i) const {
    int k = size_ + i;
    T res = data_[k];
    while (k > 1) {
      k >>= 1;
      res = Monoid::op(res, data_[k]);
    }
    return res;
  }

  // Applies a value to [l,r) range.
  void apply(int l, int r, T val) {
    l += size_;
    r += size_;
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) {
        data_[l] = Monoid::op(data_[l], val);
        ++l;
      }
      if (r & 1) {
        --r;
        data_[r] = Monoid::op(data_[r], val);
      }
    }
  }

 private:
  int size_;
  std::vector<T> data_;
};

struct Max {
  using T = pair<int, int>;  // (time, color)
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return {-1, 0}; }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  Graph g(N);
  g.input_undirected(N - 1);
  HLD hld(g.adj);
  DualSegTree<Max> seg(N);
  REP(i, Q) {
    int u, v, c;
    cin >> u >> v >> c;
    --u;
    --v;
    for (auto [l, r] : hld.edge_ranges_on_path(u, v)) {
      seg.apply(l, r, {i, c});
    }
  }

  vector<int> edge_colors(N - 1);
  REP(v, N) {
    if (not hld.parent[v].has_value()) continue;
    int e = g.edge_id[{hld.parent[v].value(), v}];
    int i = hld.node_to_ord[v];
    edge_colors[e] = seg[i].second;
  }
  for (auto color : edge_colors) cout << color << '\n';
}
