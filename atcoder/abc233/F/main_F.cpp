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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

struct UnionFind {
  int n_;
  std::vector<int> parent_;  // negative: size

  explicit UnionFind(int n) : n_(n), parent_(n, -1) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent_[x] < -parent_[y]) std::swap(x, y);
    parent_[x] += parent_[y];
    parent_[y] = x;
    return true;
  }

  int find(int v) {
    if (parent_[v] < 0) return v;
    return parent_[v] = find(parent_[v]);
  }

  int size(int v) { return -parent_[find(v)]; }

  bool same(int x, int y) { return find(x) == find(y); }
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

template<bool reshape_graph = false>
struct DFSTree {
  using Graph = std::vector<std::vector<int>>;

  int n_;
  int root_;
  Graph g_;
  std::vector<int> parent_;
  std::vector<int> depth_;

  // edge(u,v) where v is an ancestor of u in the DFS tree.
  std::set<std::pair<int, int>> back_edges_;

  explicit DFSTree(Graph g, int root = 0)
      : n_(int(g.size())),
        root_(root),
        g_(std::move(g)),
        parent_(n_, -1),
        depth_(n_, -1) {
    dfs(root_, -1, 0);
  }

  bool is_connected(int u, int v) const {
    if (depth_[u] < depth_[v]) std::swap(u, v);
    return (parent_[u] == v) or back_edges_.count({u, v});
  }

 private:
  void dfs(int v, int p, int d) {
    parent_[v] = p;
    depth_[v] = d;
    std::set<int> non_neighbors;
    for (auto u: g_[v]) {
      if (u == p) {  // parent
        continue;
      }
      if (depth_[u] == -1) {  // child
        dfs(u, v, d + 1);
        continue;
      }
      if constexpr (reshape_graph) {
        non_neighbors.insert(u);
      }
      if (depth_[u] < d) {  // backward
        back_edges_.emplace(v, u);
      } else {
        // forward
      }
    }
    if constexpr (reshape_graph) {
      auto &adj = g_[v];
      adj.erase(std::remove_if(adj.begin(), adj.end(),
                               [&](int u) { return non_neighbors.count(u); }),
                adj.end());
    }
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
        max_bits(bit_width(n)),
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
  static int bit_width(unsigned x) {
    if (x == 0) return 0;
    return std::numeric_limits<unsigned>::digits - __builtin_clz(x);
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

optional<vector<int>> solve() {
  int n = in;
  vector<int> P = in(n);
  for (auto &x: P) --x;
  vector<int> revp(n);
  REP(i, n) revp[P[i]] = i;

  UnionFind uf(n);
  vector<vector<int>> g(n);
  map<pair<int, int>, int> edge_id;
  int m = in;
  REP(i, m) {
    int a = in, b = in;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
    uf.unite(a, b);
    edge_id[{a, b}] = i + 1;
    edge_id[{b, a}] = i + 1;
  }
  vector<vector<int>> compo(n);
  REP(v, n) {
    int r = uf.find(v);
    compo[r].push_back(v);
  }

  vector<int> ans;
  vector<int> done(n, 0);

  REP(r, n) {
    if (compo[r].empty()) continue;
    sort(ALL(compo[r]));
    vector<int> ps;
    for (int x: compo[r]) {
      ps.push_back(P[x]);
    }
    sort(ALL(ps));
    if (ps != compo[r]) {
      return nullopt;
    }
  }
  REP(r, n) {
    if (compo[r].empty()) continue;
    const auto &subpos_to_pos = compo[r];
    map<int, int> pos_to_subpos;
    REP(j, subpos_to_pos.size()) {
      pos_to_subpos[subpos_to_pos[j]] = j;
    }
    vector<vector<int>> subg(subpos_to_pos.size());
    REP(x, subpos_to_pos.size()) {
      for (auto w: g[subpos_to_pos[x]]) {
        auto it = pos_to_subpos.find(w);
        assert(it != pos_to_subpos.end());
        subg[x].push_back(it->second);
      }
    }
    DFSTree<true> dfst(subg);
    AncestorBinaryLifting abl(dfst.g_);

    auto swap_edges = [&](int y, int a) -> vector<tuple<int, int, int>> {
      vector<tuple<int, int, int>> path;
      while (y != a) {
        int q = abl.parent(y);
        assert(q >= 0);
        int vy = subpos_to_pos[y];
        int vq = subpos_to_pos[q];
        int e = edge_id[{vy, vq}];
        path.push_back({e, vy, vq});
        y = q;
      }
      return path;
    };

    auto dfs1 = Rec([&](auto &self, int x, int p) -> void {
      for (auto &y: dfst.g_[x]) {
        if (y == p) continue;
        self(y, x);
      }
      int vx = subpos_to_pos[x];
      if (P[vx] == vx) {
        done[vx] = true;
        return;
      }
      assert(not done[revp[vx]]);
      int y = pos_to_subpos[revp[vx]];
      assert(y != x);
      int a = abl.lca(x, y);
      auto path1 = swap_edges(y, a);
      auto path2 = swap_edges(x, a);
      reverse(ALL(path2));
      for (auto[e, v1, v2]: path1) {
        ans.push_back(e);
        int val1 = P[v1];
        int val2 = P[v2];
        swap(P[v1], P[v2]);
        swap(revp[val1], revp[val2]);
      }
      for (auto[e, v1, v2]: path2) {
        ans.push_back(e);
        int val1 = P[v1];
        int val2 = P[v2];
        swap(P[v1], P[v2]);
        swap(revp[val1], revp[val2]);
      }
      done[vx] = true;
    });
    dfs1(0, -1);
  }
  DUMP(P);
  DUMP(revp);
  assert(ssize(ans) <= 500000);
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  auto ans = (solve());
  if (not ans) {
    print(-1);
    exit(0);
  }
  print(ans->size());
  print_seq(*ans);
}
