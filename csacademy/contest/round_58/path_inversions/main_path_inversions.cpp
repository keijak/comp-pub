#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <stack>
#include <cassert>
#include <limits>
#include <functional>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
using Int = long long;

using namespace std;

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

struct CentroidTree {
  int n;                    // number of nodes.
  int root;                 // root of the centroid tree.
  std::vector<int> parent;  // parent of each node in the centroid tree.
  std::vector<std::vector<int>> graph;  // adjacent nodes in the centroid tree.

  explicit CentroidTree(std::vector<std::vector<int>> g)
      : n(g.size()), root(-1), parent(n, -1), graph(n) {
    std::vector<std::set<int>> gd(n);
    std::vector<int> subsize(n, 0);
    for (int v = 0; v < n; ++v) {
      gd[v].insert(g[v].begin(), g[v].end());
    }
    const int start_node = 0;
    build(start_node, -1, gd, subsize);
    for (int v = 0; v < n; ++v) {
      const int p = parent[v];
      if (p == -1) {
        root = v;
      } else {
        graph[p].push_back(v);
        graph[v].push_back(p);
      }
    }
  }

 private:
  void build(int v, int p, std::vector<std::set<int>> &gd,
             std::vector<int> &subsize) {
    const int sz = dfs_subsize(v, p, gd, subsize);  // fill subsize
    const int c = centroid(v, p, sz, gd, subsize);  // find centroid
    parent[c] = p;

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

  int centroid(int v, int p, int sz, const std::vector<std::set<int>> &gd,
               const std::vector<int> &subsize) {
    for (auto u: gd[v]) {
      if (u == p) continue;
      if (subsize[u] > sz / 2) {
        return centroid(u, v, sz, gd, subsize);
      }
    }
    return v;
  }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, K;
  cin >> n >> K;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }

  CentroidTree ct(g);
  AncestorBinaryLifting abl(g);

  function<Int(int, int)> calc = [&](int v, int p) -> Int {
    Int ret = 0;
    for (auto u: ct.graph[v]) {
      if (u == p) continue;
      ret += calc(u, v);
    }
    map<int, Int> freq;
    freq[0] = 1;
    for (auto u: ct.graph[v]) {
      if (u == p) continue;
      stack<int> stk;
      map<int, Int> freq2;
      stk.push(u);
      while (not stk.empty()) {
        int w = stk.top();
        stk.pop();
        int d = abl.distance(w, v);
        if (d <= K) {
          ret += freq[K - d];
          ++freq2[d];
        }
        for (auto x: ct.graph[w]) {
          if (x == ct.parent[w]) continue;
          stk.push(x);
        }
      }
      if (freq.size() < freq2.size()) {
        freq.swap(freq2);
      }
      for (auto kv: freq2) {
        freq[kv.first] += kv.second;
      }
    }
    return ret;
  };
  Int kpath_cnt = calc(ct.root, -1);
  Int ans = kpath_cnt * (kpath_cnt - 1) / 2;
  cout << ans << endl;
}
