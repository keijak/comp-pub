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
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
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

struct SuccinctBitVector {
  int size_;
  int blocks_;
  std::vector<unsigned> bit_, sum_;

  SuccinctBitVector() : size_(0), blocks_(0) {};

  explicit SuccinctBitVector(int size)
      : size_(size),
        blocks_((size_ + 31) >> 5),
        bit_(blocks_, 0U),
        sum_(blocks_, 0U) {}

  void set(int k) { bit_[k >> 5] |= 1U << (k & 31); }

  void build() {
    sum_[0] = 0U;
    for (int i = 1; i < blocks_; ++i) {
      sum_[i] = sum_[i - 1] + __builtin_popcount(bit_[i - 1]);
    }
  }

  // O(1)
  bool operator[](int k) const { return (bit_[k >> 5] >> (k & 31)) & 1; }

  // O(1)
  int rank(int k) const {
    return (sum_[k >> 5] +
        __builtin_popcount(bit_[k >> 5] & ((1U << (k & 31)) - 1)));
  }

  // O(1)
  int rank(bool val, int k) const { return (val ? rank(k) : k - rank(k)); }

  // O(log(n))
  int select(bool val, int k) const {
    if (k < 0 || rank(val, size_) <= k) return (-1);
    int low = 0, high = size_;
    while (high - low > 1) {
      int mid = (low + high) >> 1;
      if (rank(val, mid) >= k + 1)
        high = mid;
      else
        low = mid;
    }
    return (high - 1);
  }

  // O(log(n))
  int select(bool val, int i, int l) const {
    return select(val, i + rank(val, l));
  }
};

template<class T = unsigned, int kBitWidth = std::numeric_limits<T>::digits>
struct WaveletMatrix {
  static_assert(std::is_unsigned<T>::value, "Requires unsigned type");

  int size_;
  std::array<SuccinctBitVector, kBitWidth> matrix;
  std::array<int, kBitWidth> mid;

  WaveletMatrix() = default;

  explicit WaveletMatrix(std::vector<T> v) : size_(v.size()) {
    std::vector<T> l(size_), r(size_);
    for (int level = kBitWidth - 1; level >= 0; --level) {
      matrix[level] = SuccinctBitVector(size_ + 1);
      int left = 0, right = 0;
      for (int i = 0; i < size_; ++i) {
        if (((v[i] >> level) & 1)) {
          matrix[level].set(i);
          r[right++] = v[i];
        } else {
          l[left++] = v[i];
        }
      }
      mid[level] = left;
      matrix[level].build();
      v.swap(l);
      for (int i = 0; i < right; ++i) {
        v[left + i] = r[i];
      }
    }
  }

  inline int size() const { return size_; }

  // access(i): the value at i (0-indexed).
  T operator[](int i) const {
    T ret = 0;
    for (int level = kBitWidth - 1; level >= 0; --level) {
      bool f = matrix[level][i];
      if (f) ret |= T(1) << level;
      i = matrix[level].rank(f, i) + mid[level] * f;
    }
    return ret;
  }

  // count i s.t. (0 <= i < r) && v[i] == x
  int rank(const T &x, int r) const {
    int l = 0;
    for (int level = kBitWidth - 1; level >= 0; --level) {
      std::tie(l, r) = succ((x >> level) & 1, l, r, level);
    }
    return r - l;
  }

  // k-th(0-indexed) smallest number in v[l,r)
  T kth_smallest(int l, int r, int k) const {
    assert(0 <= k && k < r - l);
    T ret = 0;
    for (int level = kBitWidth - 1; level >= 0; --level) {
      int cnt = matrix[level].rank(false, r) - matrix[level].rank(false, l);
      bool f = cnt <= k;
      if (f) {
        ret |= T(1) << level;
        k -= cnt;
      }
      std::tie(l, r) = succ(f, l, r, level);
    }
    return ret;
  }

  // k-th(0-indexed) largest number in v[l,r)
  T kth_largest(int l, int r, int k) const {
    return kth_smallest(l, r, r - l - k - 1);
  }

  // count i s.t. (l <= i < r) && (v[i] < upper)
  int range_freq(int l, int r, T upper) const {
    int ret = 0;
    for (int level = kBitWidth - 1; level >= 0; --level) {
      bool f = ((upper >> level) & 1);
      if (f) ret += matrix[level].rank(false, r) - matrix[level].rank(false, l);
      std::tie(l, r) = succ(f, l, r, level);
    }
    return ret;
  }

  // count i s.t. (l <= i < r) && (lower <= v[i] < upper)
  int range_freq(int l, int r, T lower, T upper) const {
    return range_freq(l, r, upper) - range_freq(l, r, lower);
  }

  // max v[i] s.t. (l <= i < r) && (v[i] < upper)
  T prev_value(int l, int r, T upper) const {
    int cnt = range_freq(l, r, upper);
    return cnt == 0 ? T(-1) : kth_smallest(l, r, cnt - 1);
  }

  // min v[i] s.t. (l <= i < r) && (lower <= v[i])
  T next_value(int l, int r, T lower) const {
    int cnt = range_freq(l, r, lower);
    return cnt == r - l ? T(-1) : kth_smallest(l, r, cnt);
  }

 private:
  std::pair<int, int> succ(bool f, int l, int r, int level) const {
    return {matrix[level].rank(f, l) + mid[level] * f,
            matrix[level].rank(f, r) + mid[level] * f};
  }
};

// Binary search.
// Returns the boundary argument which satisfies pred(x).
//
// Usage:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template<class F>
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
  WaveletMatrix<unsigned> wm(move(sa));

  const unsigned amax = *max_element(ALL(a));
  REP(q, Q) {
    INPUT(int, v, w, L);
    --v, --w;
    auto ranges = hld.node_ranges_on_path(v, w);
    i64 res = bisect(amax + 1, 0, [&](unsigned x) -> bool {
      int count = 0;
      for (const auto&[l, r] : ranges) {
        int c = wm.range_freq(l, r, (unsigned) x + 1);
        count += c;
      }
      return count >= L;
    });
    print(res);
  }
}
