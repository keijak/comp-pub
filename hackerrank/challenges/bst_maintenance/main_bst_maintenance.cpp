#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;
struct CentroidTree {
  int n_;
  std::vector<int> cpar_;  // parent of each node in the centroid tree

  explicit CentroidTree(std::vector<std::vector<int>> g, int root = 0)
      : n_(g.size()), cpar_(n_, -1) {
    std::vector<std::set<int>> gd(n_);
    std::vector<int> subsize(n_, 0);
    for (int v = 0; v < n_; ++v) {
      gd[v].insert(g[v].begin(), g[v].end());
    }
    build(root, -1, gd, subsize);
  }

 private:
  void build(int v, int p, std::vector<std::set<int>> &gd,
             std::vector<int> &subsize) {
    const int n = dfs_subsize(v, p, gd, subsize);  // fill subsize
    const int c = centroid(v, p, n, gd, subsize);  // find centroid
    cpar_[c] = p;

    std::vector<int> tmp(gd[c].begin(), gd[c].end());
    for (auto u: tmp) {
      gd[c].erase(u);
      gd[u].erase(c);
      build(u, c, gd, subsize);
    }
  }

  int dfs_subsize(int v, int p, const std::vector<std::set<int>> &gd,
                  std::vector<int> &subsize) {
    subsize[v] = 1;
    for (auto u: gd[v]) {
      if (u == p) continue;
      subsize[v] += dfs_subsize(u, v, gd, subsize);
    }
    return subsize[v];
  }

  int centroid(int v, int p, int n, const std::vector<std::set<int>> &gd,
               const std::vector<int> &subsize) {
    for (auto u: gd[v]) {
      if (u == p) continue;
      if (subsize[u] > n / 2) {
        return centroid(u, v, n, gd, subsize);
      }
    }
    return v;
  }
};

struct AncestorBinaryLifting {
  using G = vector<vector<int>>;

  const int n;         // number of nodes
  const int max_bits;  // max upper lookup (2^max_bits)
  const int root;      // root node
  const G &adj;
  vector<vector<int>> upper;  // 2^k upper node
  vector<int> depth;

  explicit AncestorBinaryLifting(const G &g, int root = 0)
      : n(g.size()),
        max_bits(power2_ceil(n + 1)),
        root(root),
        adj(g),
        upper(max_bits, vector<int>(n)),
        depth(n, 0) {
    // Build `depth` and `upper[0]`.
    // The root node's parent is itself.
    dfs(root, root);

    // Build `upper[k]` for k > 0.
    for (int k = 0; k + 1 < max_bits; ++k) {
      for (int v = 0; v < n; ++v) {
        upper[k + 1][v] = upper[k][upper[k][v]];
      }
    }
  }

  // Returns the parent of v.
  // The root's parent is the root itself.
  int parent(int v) const { return upper[0][v]; }

  // Returns i-th ancestor of v.
  // - 0th ancestor is v itself.
  // - 1st ancestor is the parent.
  // - (2^k)-th ancestor is `upper[k][v]`.
  int ancestor(int v, int i) const {
    int x = v;
    for (int k = max_bits - 1; k >= 0; --k) {
      if (i & (1 << k)) {
        x = upper[k][x];
      }
    }
    return x;
  }

  // Returns the node ID of the lowest common ancestor.
  int lca(int u, int v) const {
    if (depth[u] > depth[v]) swap(u, v);

    // Move up `v` so both nodes have the same depth.
    int ddiff = depth[v] - depth[u];
    v = ancestor(v, ddiff);
    if (u == v) return u;

    // Move up both nodes but still keep them below the LCA.
    for (int k = max_bits - 1; k >= 0; --k) {
      if (upper[k][u] != upper[k][v]) {
        u = upper[k][u];
        v = upper[k][v];
      }
    }
    // Now both nodes are direct children of the LCA.
    const int pu = parent(u), pv = parent(v);
    assert(pu == pv);
    return pu;
  }

  // Returns the distance (number of edges) between two nodes.
  int distance(int u, int v) const {
    int p = lca(u, v);
    return (depth[u] - depth[p]) + (depth[v] - depth[p]);
  }

  // Binary search.
  // Returns the minimum number of steps to reach an ancestor
  // that satisfies `pred(a)`.
  template<class F>
  optional<int> min_steps(int start, F pred) const {
    static_assert(std::is_invocable_r_v<bool, F, int>);
    if (pred(start)) return 0;
    if (not pred(root)) return nullopt;
    int max_false = 0;
    int v = start;
    for (int d = max_bits - 1; d >= 0; --d) {
      auto u = upper[d][v];
      if (pred(u)) continue;
      max_false += 1 << d;
      v = u;
    }
    int min_true = max_false + 1;
    return min_true;
  }

 private:
  static int power2_ceil(int n) {
    int b = 1;
    while ((1u << b) < (unsigned) n) ++b;
    return b;
  }

  void dfs(int v, int p) {
    upper[0][v] = p;  // parent
    for (auto u: adj[v]) {
      if (u == p) continue;
      depth[u] = depth[v] + 1;
      dfs(u, v);
    }
  }
};

template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

struct Node {
  int id = -1;
  Int val = 0;
  Node *par = nullptr;
  Node *l = nullptr;
  Node *r = nullptr;
  Node() = default;
};

auto solve() {
  const int n = in;
  vector<int> a = in(n);
  Node root{0, a[0]};
  map<int, pair<Node *, bool>> bst;
  bst[a[0]] = {&root, true};
  const int bottom = -kBigVal<int>;
  bst[bottom] = {&root, false};
  vector<Node *> nodes(n, nullptr);
  nodes[0] = &root;

  auto dump_tree = Rec([&](auto &f, Node *node, int depth) -> void {
    if (node == nullptr) return;
    f(node->l, depth + 1);
    cout << string(depth * 4, ' ') << node->val << endl;
    f(node->r, depth + 1);
  });

  REP(i, 1, n) {
    int x = a[i];
    Node *node = new Node{i, x};
    nodes[i] = node;
    auto it = prev(bst.upper_bound(x));
    Node *par = it->second.first;
    node->par = par;
    if (it->second.second) {
      par->r = node;
    } else {
      par->l = node;
    }
    bst[x] = {node, true};
    it->second = {node, false};
    cout << "# " << i << " #" << endl;
    //dump_tree(&root, 0);
  }

  vector<vector<int>> g(n);
  REP(v, 1, n) {
    int p = nodes[v]->par->id;
    g[v].push_back(p);
    g[p].push_back(v);
  }
  AncestorBinaryLifting abl(g, 0);
  CentroidTree ct(g, 0);
  DUMP(ct.cpar_);
  DUMP(a);

  vector<Int> distsum(n);
  vector<int> node_counts(n);
  REP(i, n) {
    Int ans = 0;
    Int ds = 0;
    int nc = 0;
    for (int v = i; v != -1; v = ct.cpar_[v]) {
      int oc = node_counts[v] - nc;
      Int od = distsum[v] - ds;

      ans += abl.distance(i, v) * distsum[v];
    }
    print(ans);
    for (int v = i; v != -1; v = ct.cpar_[v]) {
      node_counts[v] += 1;
      distsum[v] += abl.distance(i, v);
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    (solve());
  }
}
