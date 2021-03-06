#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
using namespace std;

template <typename T>
using V = vector<T>;

struct DoublingLCA {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G adj;
  vector<vector<int>> upper;  // 2^k upper node
  vector<int> depth;

  explicit DoublingLCA(G g, int root = 0)
      : n(g.size()), adj(move(g)), upper(K, vector<int>(n, -1)), depth(n) {
    depth[root] = 0;
    dfs(0, -1);
    for (int k = 0; k + 1 < K; k++) {
      for (int v = 0; v < n; ++v) {
        if (upper[k][v] < 0) {
          upper[k + 1][v] = -1;
        } else {
          upper[k + 1][v] = upper[k][upper[k][v]];
        }
      }
    }
  }

  // Returns the node ID of the lowest common ancestor.
  int lca(int u, int v) const {
    if (depth[u] > depth[v]) swap(u, v);
    for (int k = 0; k < K; k++) {
      if ((depth[v] - depth[u]) & (1 << k)) {
        v = upper[k][v];
      }
    }
    if (u == v) return u;
    for (int k = K - 1; k >= 0; k--) {
      if (upper[k][u] != upper[k][v]) {
        u = upper[k][u];
        v = upper[k][v];
      }
    }
    return upper[0][u];
  }

  int parent(int v) const { return upper[0][v]; }

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

  static const int K = 30;  // max upper lookup (2^K)
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  V<V<int>> g(N);
  REP(i, N - 1) {
    int p;
    cin >> p;
    g[p].push_back(i + 1);
  }

  DoublingLCA lca(move(g));
  REP(i, Q) {
    int u, v;
    cin >> u >> v;
    cout << lca.lca(u, v) << '\n';
  }
}
