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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

struct AncestorBinaryLifting {
  using G = vector<vector<int>>;
  static const int K = 30;  // max upper lookup (2^K)

  const int n;  // number of nodes
  G adj;
  vector<vector<int>> upper;  // 2^k upper node
  vector<int> depth;

  explicit AncestorBinaryLifting(G g, int root = 0)
      : n(g.size()), adj(move(g)), upper(K, vector<int>(n, -1)), depth(n) {
    depth[root] = 0;

    // Build `depth` and `upper[0]`.
    dfs(0, -1);

    // Build `upper[k]` for k > 0.
    for (int k = 0; k + 1 < K; ++k) {
      for (int v = 0; v < n; ++v) {
        if (upper[k][v] < 0) {
          upper[k + 1][v] = -1;
        } else {
          upper[k + 1][v] = upper[k][upper[k][v]];
        }
      }
    }
  }

  int parent(int v) const { return upper[0][v]; }

  // Returns i-th ancestor of v.
  // - parent is the 1st ancestor.
  // - `upper[k][v]`is the (2^k)-th ancestor.
  int ancestor(int v, int i) const {
    assert(i < (1 << K));
    for (int k = K - 1; k >= 0; --k) {
      if (i & (1 << k)) {
        v = upper[k][v];
      }
    }
    return v;
  }

  // Returns the node ID of the lowest common ancestor.
  int lca(int u, int v) const {
    if (depth[u] > depth[v]) swap(u, v);
    int ddiff = depth[v] - depth[u];

    // Move up `v` so both nodes have the same depth.
    for (int k = K - 1; k >= 0; --k) {
      if (ddiff & (1 << k)) {
        v = upper[k][v];
      }
    }
    if (u == v) return u;

    // Move up both nodes but still keep them below the LCA.
    for (int k = K - 1; k >= 0; --k) {
      if (upper[k][u] != upper[k][v]) {
        u = upper[k][u];
        v = upper[k][v];
      }
    }

    // Now both nodes are direct children of the LCA.
    return parent(u);
  }

  // Returns the distance (number of edges) between two nodes.
  int distance(int u, int v) const {
    int p = lca(u, v);
    return (depth[u] - depth[p]) + (depth[v] - depth[p]);
  }

 private:
  void dfs(int v, int p) {
    upper[0][v] = p;  // parent
    for (auto u : adj[v]) {
      if (u == p) continue;
      depth[u] = depth[v] + 1;
      dfs(u, v);
    }
  }
};

struct CentroidTree {
  int n_;
  vector<int> cpar_;  // parent of each node in the centroid tree

  explicit CentroidTree(vector<vector<int>> g, int root = 0)
      : n_(g.size()), cpar_(n_, -1) {
    vector<set<int>> gd(n_);
    vector<int> subsize(n_, 0);
    REP(v, n_) { gd[v].insert(ALL(g[v])); }
    build(root, -1, gd, subsize);
  }

 private:
  void build(int v, int p, vector<set<int>> &gd, vector<int> &subsize) {
    const int n = dfs_subsize(v, p, gd, subsize);  // fill subsize
    const int c = centroid(v, p, n, gd, subsize);  // find centroid
    cpar_[c] = p;

    vector<int> tmp(gd[c].begin(), gd[c].end());
    for (auto u : tmp) {
      gd[c].erase(u);
      gd[u].erase(c);
      build(u, c, gd, subsize);
    }
  }

  int dfs_subsize(int v, int p, const vector<set<int>> &gd,
                  vector<int> &subsize) {
    subsize[v] = 1;
    for (auto u : gd[v]) {
      if (u == p) continue;
      subsize[v] += dfs_subsize(u, v, gd, subsize);
    }
    return subsize[v];
  }

  int centroid(int v, int p, int n, const vector<set<int>> &gd,
               const vector<int> &subsize) {
    for (auto u : gd[v]) {
      if (u == p) continue;
      if (subsize[u] > n / 2) {
        return centroid(u, v, n, gd, subsize);
      }
    }
    return v;
  }
};

void solve() {
  static constexpr int BIG = 1e7;
  int n, m;
  cin >> n >> m;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  AncestorBinaryLifting abl(g);
  CentroidTree ct(g);
  vector<int> mindist(n, BIG);  // min distance in the subtree

  auto paint = [&](int v) -> void {
    for (int p = v; p >= 0; p = ct.cpar_[p]) {
      const int d = abl.distance(v, p);
      chmin(mindist[p], d);
    }
  };
  paint(0);

  REP(q, m) {
    int t, v;
    cin >> t >> v;
    --v;
    if (t == 1) {
      paint(v);
    } else {
      int ans = BIG;
      for (int p = v; p >= 0; p = ct.cpar_[p]) {
        const int d = abl.distance(v, p);
        chmin(ans, mindist[p] + d);
      }
      assert(ans < BIG);
      cout << ans << "\n";
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
