#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
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

void print() { std::cout << "\n"; }
template <class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template <typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template <typename T, typename... Ts>
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
    for (auto [l, r] : edge_ranges_on_path(u, v)) {
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

template <typename T = unsigned, int kBitWidth = std::numeric_limits<T>::digits>
struct PersistentBinaryTrie {
  static_assert(std::is_unsigned<T>::value, "Requires unsigned type");

 public:
  struct Node;
  using NodePtr = Node *;

  struct Node {
    int leaf_count;
    std::array<NodePtr, 2> child;
    Node() : leaf_count(0), child{nullptr, nullptr} {}
  };
  NodePtr root_;  // The root node.

  struct NodePool {
    static constexpr size_t kInitBlockSize = 1u << 12;
    static constexpr double kBlockSizeGrowthRate = 1.5;  // Decrease if MLE.

    std::vector<std::unique_ptr<Node[]>> blocks_;
    size_t bsize_;
    size_t bi_;
    size_t ni_;

    NodePool() : bsize_(kInitBlockSize), bi_(0), ni_(0) {
      blocks_.emplace_back(new Node[kInitBlockSize]);
    }

    NodePtr new_node() {
      if (ni_ == bsize_) {
        bi_++;
        ni_ = 0;
        bsize_ *= kBlockSizeGrowthRate;
        blocks_.emplace_back(new Node[bsize_]);
      }
      return &blocks_[bi_][ni_++];
    }
  };
  NodePool *pool_;

  PersistentBinaryTrie() : root_(nullptr), pool_(NO_DELETE()) {}
  explicit PersistentBinaryTrie(NodePool *p) : root_(nullptr), pool_(p) {}
  PersistentBinaryTrie(NodePtr r, NodePool *p) : root_(r), pool_(p) {}

  int size() const { return root_ ? root_->leaf_count : 0; }

  bool empty() const { return size() == 0; }

  PersistentBinaryTrie insert(T val) const {
    return PersistentBinaryTrie(insert_internal(root_, val), pool_);
  }

  // Removes one element of `val`.
  // At least one `val` must exist in the trie. Check `trie.count(val) > 0`.
  PersistentBinaryTrie erase_one(T val) const {
    return PersistentBinaryTrie(erase_internal(root_, val), pool_);
  }

  // Returns the element x in the trie that maximizes `x ^ xor_mask`.
  T max_element(T xor_mask = 0) const { return get_min(root_, ~xor_mask); }

  // Returns the element x in the trie that minimizes `x ^ xor_mask`.
  T min_element(T xor_mask = 0) const { return get_min(root_, xor_mask); }

  // Returns k-th (0-indexed) smallest value.
  T operator[](int k) const {
    assert(0 <= k and k < size());
    return get_internal(root_, k);
  }

  // Returns k-th (0-indexed) largest value.
  T kth_largest(int k) const {
    const int i = size() - k - 1;
    return (*this)[i];
  }

  // Returns the minimum index i s.t. trie[i] >= val.
  int lower_bound(T val) const { return count_less(root_, val); }

  // Returns the minimum index i s.t. trie[i] > val.
  int upper_bound(T val) const { return count_less(root_, val + 1); }

  // Counts the number of elements that are equal to `val`.
  // Note: BinaryTrie is a multiset.
  int count(T val) const {
    if (not root_) return 0;
    NodePtr t = root_;
    for (int i = kBitWidth - 1; i >= 0; i--) {
      t = t->child[val >> i & 1];
      if (not t) return 0;
    }
    return t->leaf_count;
  }

  std::vector<T> to_vec() const {
    std::vector<T> res;
    res.reserve(size());
    to_vec_internal(root_, T(0), res);
    return res;
  }

 private:
  static NodePool *NO_DELETE() {
    static NodePool kNoDeletePool;
    return &kNoDeletePool;
  }

  NodePtr insert_internal(NodePtr t, T val, int b = kBitWidth - 1) const {
    NodePtr res = pool_->new_node();
    res->leaf_count = 1;
    if (t != nullptr) {
      res->leaf_count += t->leaf_count;
      res->child[0] = t->child[0];
      res->child[1] = t->child[1];
    }
    if (b < 0) return res;
    bool f = (val >> b) & 1;
    res->child[f] = insert_internal(res->child[f], val, b - 1);
    return res;
  }

  NodePtr erase_internal(NodePtr t, T val, int b = kBitWidth - 1) const {
    assert(t);
    if (t->leaf_count == 1) {
      return nullptr;
    }
    NodePtr res = pool_->new_node();
    res->leaf_count = t->leaf_count - 1;
    res->child[0] = t->child[0];
    res->child[1] = t->child[1];
    if (b < 0) return res;
    bool f = (val >> b) & 1;
    res->child[f] = erase_internal(res->child[f], val, b - 1);
    return res;
  }

  T get_min(NodePtr t, T xor_mask, int b = kBitWidth - 1) const {
    assert(t != nullptr);
    if (b < 0) return 0;
    bool f = (xor_mask >> b) & 1;
    f ^= not t->child[f];
    return get_min(t->child[f], xor_mask, b - 1) | (T(f) << b);
  }

  T get_internal(NodePtr t, int k, int b = kBitWidth - 1) const {
    if (b < 0) return 0;
    int m = t->child[0] ? t->child[0]->leaf_count : 0;
    return k < m ? get_internal(t->child[0], k, b - 1)
                 : get_internal(t->child[1], k - m, b - 1) | (T(1) << b);
  }

  int count_less(NodePtr t, T val, int b = kBitWidth - 1) const {
    if (not t or b < 0) return 0;
    bool f = (val >> b) & 1;
    return (f and t->child[0] ? t->child[0]->leaf_count : 0) +
           count_less(t->child[f], val, b - 1);
  }

  void to_vec_internal(NodePtr t, T val, std::vector<T> &out,
                       int b = kBitWidth - 1) const {
    if (not t) return;
    if (b < 0) {
      out.push_back(val);
      return;
    }
    if (t->child[0]) {
      to_vec_internal(t->child[0], val, out, b - 1);
    }
    if (t->child[1]) {
      to_vec_internal(t->child[1], val | (T(1) << b), out, b - 1);
    }
  }
};
using Trie = PersistentBinaryTrie<>;

// Binary search.
// Returns the boundary argument which satisfies pred(x).
//
// Usage:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template <class F>
i64 bisect(i64 true_x, i64 false_x, F pred) {
  static_assert(std::is_invocable_r_v<bool, F, i64>, "F must be: i64 -> bool");
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
  Trie empty;
  vector<Trie> trie(n);

  auto dfs = [&](auto &rec, int v, int p, const Trie &ptrie) -> void {
    trie[v] = ptrie.insert(a[v]);
    for (auto u : g[v]) {
      if (u == p) continue;
      rec(rec, u, v, trie[v]);
    }
  };
  dfs(dfs, 0, -1, empty);

  const unsigned amax = *max_element(ALL(a));
  REP(q, Q) {
    INPUT(int, v, w, L);
    --v, --w;
    int a1 = hld.lca(v, w);
    int a2 = hld.parent[a1];

    i64 res = bisect(amax + 1, 0, [&](unsigned x) -> bool {
      int c1 = trie[v].upper_bound(x);
      int c2 = trie[w].upper_bound(x);
      int d1 = trie[a1].upper_bound(x);
      int d2 = 0;
      if (a2 != -1) {
        d2 = trie[a2].upper_bound(x);
      }
      int count = c1 + c2 - d1 - d2;
      return count >= L;
    });
    print(res);
  }
}
