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

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
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
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template<typename T>
constexpr int num_bits = CHAR_BIT * sizeof(T);

// Log base 2 of the most significant bit that is set to 1.
inline int msb_log(unsigned x) {
  assert(x != 0);
  return num_bits<unsigned> - __builtin_clz(x) - 1;
}

// Range Min/Max Query based on Fischer-Heun Structure.
// - Initialization: O(n)
// - Query: O(1)
//
// Usage:
//   RMQ rmq(a.size(), [&](int i, int j){ return a[i] < a[j]; });
//   auto minval = a[rmq.fold(l, r)];
template<class BetterOp, class mask_t = unsigned>
struct RMQ {
  static_assert(std::is_integral_v<mask_t>, "mask_t must be integral");
  static_assert(std::is_unsigned_v<mask_t>, "mask_t must be unsigned");
  static_assert(std::is_invocable_r_v<bool, BetterOp, int, int>);
  static constexpr int block_size_ = num_bits<mask_t>;

  int n_;                 // sequence size
  int block_count_;       // total number of blocks
  BetterOp better_than_;  // checks if lhs is strictly better than rhs.
  std::vector<mask_t> indicator_;
  std::vector<std::vector<int>> sparse_table_;

  RMQ(int n, BetterOp better)
      : n_(n),
        block_count_((n_ + block_size_ - 1) / block_size_),
        better_than_(std::move(better)),
        indicator_(n_),
        sparse_table_(
            block_count_ == 0 ? 0 : msb_log(unsigned(block_count_)) + 1,
            std::vector<int>(block_count_)) {
    static constexpr int bufsize = block_size_ + 1;
    static int buf[bufsize];       // ring buffer [lp,rp)
    int lp = 1, rp = 1, rpm1 = 0;  // rpm1 = rp-1 (mod bufsize)

    // Build the indicator table.
    for (int r = 0; r < n_; ++r) {
      while (lp != rp and r - buf[lp] >= block_size_) {
        ++lp;
        if (lp == bufsize) lp = 0;
      }
      while (lp != rp and not better_than_(buf[rpm1], r)) {
        rp = rpm1--;
        if (rp == 0) rpm1 = bufsize - 1;
      }
      indicator_[r] = 1;
      if (lp != rp) {
        const int p = buf[rpm1];
        indicator_[r] |= (indicator_[p] << (r - p));
      }
      buf[rp] = r;
      rpm1 = rp++;
      if (rp == bufsize) rp = 0;
    }

    // Build the sparse table.
    for (int i = 0; i < block_count_; ++i) {
      sparse_table_[0][i] =
          best_index_small(std::min(block_size_ * (i + 1), n_) - 1);
    }
    for (int i = 0, height = int(sparse_table_.size()) - 1; i < height; ++i) {
      for (int j = 0; j < block_count_; ++j) {
        sparse_table_[i + 1][j] = better_index(
            sparse_table_[i][j],
            sparse_table_[i][std::min(j + (1 << i), block_count_ - 1)]);
      }
    }
  }

  // Returns the index of the best value in [l, r) (half-open interval).
  inline int fold(int l, int r) const {
    assert(l < r);
    // Internally use closed interval.
    return best_index(l, r - 1);
  }

 private:
  inline int better_index(int i, int j) const {
    return better_than_(i, j) ? i : j;
  }

  // Returns the index of the best value in [r - width, r] (closed interval).
  inline int best_index_small(int r, int width = block_size_) const {
    assert(r < n_);
    assert(width > 0);
    assert(width <= block_size_);
    mask_t ind = indicator_[r];
    if (width < block_size_) {
      ind &= (mask_t(1) << width) - 1;
    }
    return r - msb_log(ind);
  }

  // Returns the index of the best value in [l, r] (closed interval).
  inline int best_index(int l, int r) const {
    l = std::clamp(l, 0, n_ - 1);
    r = std::clamp(r, 0, n_ - 1);
    const int width = r - l + 1;
    if (width <= block_size_) {
      return best_index_small(r, width);
    }
    const int al = best_index_small(std::min(l + block_size_, n_) - 1);
    const int ar = best_index_small(r);
    int ans = better_index(al, ar);

    const int bl = l / block_size_ + 1;
    const int br = r / block_size_ - 1;
    if (bl <= br) {
      const int k = msb_log(unsigned(br - bl + 1));
      const int bl2 = br - (1 << k) + 1;
      const int am = better_index(sparse_table_[k][bl], sparse_table_[k][bl2]);
      ans = better_index(ans, am);
    }
    return ans;
  }
};

struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  const G &g;

  // Euler Tour on nodes.
  vector<int> in;   // index in the tour on entering the subtree of v
  vector<int> out;  // index in the tour on exiting the subtree of v
  vector<int> depth;
  vector<int> node_tour;
  vector<int> depth_tour;

  explicit EulerTour(const G &g, int root = 0)
      : n((int) g.size()), g(g), in(n, -1), out(n, -1), depth(n, -1) {
    node_tour.reserve(n * 2);
    depth_tour.reserve(n * 2);
    depth[root] = 0;
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    in[v] = int(node_tour.size());
    if (p != -1) depth[v] = depth[p] + 1;
    node_tour.push_back(v);
    depth_tour.push_back(depth[v]);
    for (auto u: g[v]) {
      if (u == p) continue;
      dfs(u, v);
      node_tour.push_back(v);
      depth_tour.push_back(depth[v]);
    }
    out[v] = int(node_tour.size());
  }
};

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

pair<int, int> range_lca(const EulerTour::G &g, int l, int r) {
  EulerTour et(g);
  RMQ depth_rmq(et.depth_tour.size(), [&](int i, int j) {
    return et.depth_tour[i] < et.depth_tour[j];
  });
  RMQ min_in(et.in.size(), [&](int i, int j) { return et.in[i] < et.in[j]; });
  RMQ max_in(et.in.size(), [&](int i, int j) { return et.in[i] > et.in[j]; });
  {  // query
    int mini = min_in.fold(l, r);
    int maxi = max_in.fold(l, r);
    int lca_ord = depth_rmq.fold(mini, maxi + 1);
    int lca_node = et.node_tour[lca_ord];
    int lca_depth = et.depth_tour[lca_ord];
    return {lca_node, lca_depth};
  }
}

auto solve() {
  const int n = in;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int a = in, b = in;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  EulerTour et(g);
  HLDecomp hld(g);
  RMQ depth_rmq(et.depth_tour.size(), [&](int i, int j) {
    return et.depth_tour[i] < et.depth_tour[j];
  });

  vector<int> F = in(n);
  map<int, vector<int>> components;
  REP(i, n) {
    components[F[i]].push_back(i);
  }

  vector<i64> ehist(n + 1, 0);
  for (const auto&[freq, nodes]: components) {
    if (ssize(nodes) < 2) continue;
    int mini = n + 100, maxi = -1;
    REP(i, nodes.size()) {
      int x = et.in[nodes[i]];
      chmin(mini, x);
      chmax(maxi, x);
    }
    int lca_ord = depth_rmq.fold(mini, maxi + 1);
    int lca_node = et.node_tour[lca_ord];
    for (auto v: nodes) {
      for (auto[l, r]: hld.edge_ranges_on_path(v, lca_node)) {
        ehist[l]++;
        ehist[r]--;
      }
    }
  }
  REP(i, n) {
    ehist[i + 1] += ehist[i];
  }
  int count = n - 1;
  REP(i, n + 1) {
    if (ehist[i]) --count;
  }
  return max(count, 0);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = in;
  REP(test_case, t) {
    cout << "Case #" << (test_case + 1) << ": ";
    auto ans = solve();
    print(ans);
  }
}
