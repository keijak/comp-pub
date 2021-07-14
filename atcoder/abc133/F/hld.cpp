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
                        std::ostream *os = nullptr) {
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

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", &(os << "{")) << "}";
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

// Heavy-Light Decomposition
struct HLD {
  using NodeID = int;                       // [0, n)
  using G = std::vector<std::vector<int>>;  // undirected graph
  // half-open intervals of preorder indices of nodes.
  using IntervalVec = std::vector<std::pair<int, int>>;

  int n;                                      // number of nodes in the tree
  NodeID root;                                // root of the tree
  G children;                                 // children node ids
  std::vector<std::optional<NodeID>> parent;  // parent node id (or -1)
  std::vector<int> subsize;                   // subtree size
  // "ord" is preorder index in DFS traversal. [0, n)
  std::vector<int> node_to_ord;     // node id to preorder index
  std::vector<NodeID> ord_to_node;  // preorder index to node id
  std::vector<NodeID> comp_root;    // node id to its heavy path component

  // g is an undirected graph.
  explicit HLD(G g, NodeID root = 0)
      : n(int(g.size())),
        root(root),
        children(g),
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
  // Fills `parent` and `subsize` and drops parent node ids from `children`.
  void dfs_subsize(NodeID v) {
    auto &edges = children[v];
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
    for (NodeID u : children[v]) {
      comp_root[u] = (u == children[v][0] ? comp_root[v] : u);
      dfs_hld(u, counter);
    }
  }
};

struct Edge {
  int color;
  int length;
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  HLD::G g(n);
  vector<Edge> edges(n - 1);
  map<pair<int, int>, int> edge_id;
  REP(i, n - 1) {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
    edges[i] = {c, d};
    edge_id[{a, b}] = edge_id[{b, a}] = i;
  }
  HLD hld(move(g));

  int bucket_size = std::sqrt(n) + 5;
  int bucket_count = (n + bucket_size - 1) / bucket_size;

  // color => {count, length_sum}
  vector<map<int, pair<int, i64>>> buckets(bucket_count);
  vector<i64> buckets_tot(bucket_count, 0);
  vector<const Edge *> ord_to_edge(n);
  REP(i, bucket_count) {
    auto &bucket = buckets[i];
    REP(j, bucket_size) {
      int x = i * bucket_size + j;
      if (x >= n) continue;
      int v = hld.ord_to_node[x];
      const auto &u = hld.parent[v];
      if (not u.has_value()) continue;
      int ei = edge_id[{u.value(), v}];
      const auto &e = edges[ei];
      ord_to_edge[x] = &e;
      auto &p = bucket[e.color];
      p.first++;
      p.second += e.length;
      buckets_tot[i] += e.length;
    }
    // DEBUG(i, bucket);
  }

  auto length_sum = [&](int l, int r, int x, int y) -> i64 {
    i64 res = 0;
    for (int j = l; j < r; ++j) {
      const Edge *e = ord_to_edge[j];
      if (e->color == x) {
        res += y;
      } else {
        res += e->length;
      }
    }
    return res;
  };

  REP(i, q) {
    int x, y, u, v;
    cin >> x >> y >> u >> v;
    --u, --v;
    i64 dist = 0;
    for (auto [l, r] : hld.edge_ranges_on_path(u, v)) {
      int bl = l / bucket_size;
      int br = (r - 1) / bucket_size;
      if (bl == br) {
        dist += length_sum(l, r, x, y);
      } else {
        dist += length_sum(l, (bl + 1) * bucket_size, x, y);
        dist += length_sum(br * bucket_size, r, x, y);
        for (int j = bl + 1; j < br; ++j) {
          const auto &bucket = buckets[j];
          dist += buckets_tot[j];
          auto it = bucket.find(x);
          if (it == bucket.end()) continue;
          auto [color_count, color_length] = it->second;
          dist -= color_length;
          dist += i64(color_count) * y;
        }
      }
    }
    cout << dist << "\n";
  }
}
