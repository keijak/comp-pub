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
struct is_iterable<T, std::void_t < decltype(std::begin(std::declval<T>())),
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

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// Heavy-Light Decomposition
struct HLD {
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
    int cost = 0;
    for (auto[l, r] : edge_ranges_on_path(u, v)) {
      cost += r - l;
    }
    return cost;
  }

 private:
  // Fills `parent` and `subsize` and drops parent node ids from `child`.
  void dfs_subsize(NodeID v) {
    auto &edges = child[v];
    if (parent[v] != -1) {
      auto it = std::find(edges.begin(), edges.end(), parent[v]);
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

template<typename Monoid, int NODES = 20000000>
struct PersistentSegmentTree {
  using T = typename Monoid::T;

  struct Node {
    T data;
    Node *l, *r;
    Node() {}
    Node(const T &_data) : data(_data), l(nullptr), r(nullptr) {}
  };

  Node *pool;
  int pid;
  i64 N;
  Node *nil;
  std::vector<Node *> roots;

  explicit PersistentSegmentTree(const std::vector<T> &v)
      : pid(0), nil(nullptr) {
    pool = new Node[NODES];
    nil = my_new(Monoid::id());
    nil->l = nil->r = nil;
    roots.reserve(262144);
    roots.push_back(build(v));
  }

  explicit PersistentSegmentTree(i64 N_) : pid(0), N(N_), nil(nullptr) {
    pool = new Node[NODES];
    nil = my_new(Monoid::id());
    nil->l = nil->r = nil;
    roots.reserve(262144);
    roots.push_back(nil);
  }

  Node *my_new(const T &data) {
    pool[pid].data = data;
    pool[pid].l = pool[pid].r = nil;
    return &(pool[pid++]);
  }

  Node *merge(Node *l, Node *r) {
    pool[pid].data = Monoid::op(l->data, r->data);
    pool[pid].l = l;
    pool[pid].r = r;
    return &(pool[pid++]);
  }

  Node *build(const std::vector<T> &v) {
    N = (i64) v.size();
    return build(0, (i64) v.size(), v);
  }

  Node *build(i64 l, i64 r, const std::vector<T> &v) {
    if (l + 1 == r) return my_new(v[l]);
    i64 m = (l + r) >> 1;
    return merge(build(l, m, v), build(m, r, v));
  }

  Node *set(Node *n, i64 k, const T &x) {
    Node *root = set_(k, x, n, 0, N);
    roots.push_back(root);
    return root;
  }

  T fold(Node *n, i64 a, i64 b) { return fold_(a, b, n, 0, N); }
  T get(Node *n, i64 k) { return fold_(k, k + 1, n, 0, N); }

  Node *new_tree() { return nil; }

 private:
  Node *set_(i64 a, const T &x, Node *n, i64 l, i64 r) {
    if (l + 1 == r) return my_new(x);
    i64 m = (l + r) >> 1;
    if (a < m) return merge(set_(a, x, n->l, l, m), n->r);
    return merge(n->l, set_(a, x, n->r, m, r));
  }

  T fold_(i64 a, i64 b, Node *n, i64 l, i64 r) {
    if (n == nil) return Monoid::id();
    if (r <= a or b <= l) return Monoid::id();
    if (a <= l and r <= b) return n->data;
    i64 m = (l + r) >> 1;
    return Monoid::op(fold_(a, b, n->l, l, m), fold_(a, b, n->r, m, r));
  }
};

struct SumOp {
  using T = unsigned;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T
  id() { return 0; }
};

// Binary search.
// Returns the boundary argument which satisfies pred(x).
//
// Usage:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template<class F>
i64 bisect(i64 true_x, i64 false_x, F pred) {
  static_assert(std::is_invocable_r_v < bool, F, i64 > , "F must be: i64 -> bool");
  while (std::abs(true_x - false_x) > 1) {
    i64 mid = (true_x + false_x) / 2;
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}

unsigned f(unsigned x, unsigned y) {
  return x + y;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  INPUT(int, n, Q);
  vector<unsigned> a(n);
  cin >> a;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    INPUT(int, u, v);
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  HLD hld(g);
  vector<unsigned> sa(n);
  REP(i, n) {
    int j = hld.node_to_ord[i];
    sa[j] = a[i];
  }
  using Tree = PersistentSegmentTree<SumOp>;
  const unsigned amax = *max_element(ALL(a));
  Tree tree(amax + 1);
  vector < Tree::Node * > nodes(n);

  auto dfs = [&](auto &rec, int v, int p, Tree::Node *pnode) -> void {
    Tree::Node *cur_node = tree.set(pnode, a[v], tree.get(pnode, a[v]) + 1);
    nodes[v] = cur_node;
    for (auto u : g[v]) {
      if (u == p) continue;
      rec(rec, u, v, cur_node);
    }
  };
  dfs(dfs, 0, -1, tree.new_tree());

  REP(q, Q) {
    INPUT(int, v, w, L);
    --v, --w;
    int a1 = hld.lca(v, w);
    int a2 = hld.parent[a1];

    i64 res = bisect(amax + 1, 0, [&](unsigned x) -> bool {
      int c1 = tree.fold(nodes[v], 0LL, x + 1);
      int c2 = tree.fold(nodes[w], 0LL, x + 1);
      int d1 = tree.fold(nodes[a1], 0LL, x + 1);
      int d2 = 0;
      if (a2 != -1) {
        d2 = tree.fold(nodes[a2], 0LL, x + 1);
      }
      int count = c1 + c2 - d1 - d2;
      return count >= L;
    });
    print(res);
  }
}
