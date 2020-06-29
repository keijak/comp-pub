// Lowest Common Ancestor
class LCA {
 public:
  explicit LCA(int n)
      : n(n), adj(n), parent(K, vector<int>(n, -1)), depth(n, -1) {}

  void add_edge(int x, int y) {
    adj[x].push_back(y);
    adj[y].push_back(x);
  }

  void build() {
    dfs(0, -1, 0);
    for (int k = 0; k + 1 < K; k++) {
      REP(v, n) {
        if (parent[k][v] < 0) {
          parent[k + 1][v] = -1;
        } else {
          parent[k + 1][v] = parent[k][parent[k][v]];
        }
      }
    }
  }

  int query(int u, int v) {
    if (depth[u] > depth[v]) swap(u, v);
    for (int k = 0; k < K; k++) {
      if ((depth[v] - depth[u]) & (1 << k)) {
        v = parent[k][v];
      }
    }
    if (u == v) return u;
    for (int k = K - 1; k >= 0; k--) {
      if (parent[k][u] != parent[k][v]) {
        u = parent[k][u];
        v = parent[k][v];
      }
    }
    return parent[0][u];
  }

 private:
  void dfs(int v, int p, int d) {
    parent[0][v] = p;
    depth[v] = d;
    for (auto x : adj[v]) {
      if (x == p) continue;
      dfs(x, v, d + 1);
    }
  }

  static const int K = 30;  // max parent lookup (2^K)
  const int n;              // number of nodes
  vector<vector<int>> adj;
  vector<vector<int>> parent;
  vector<int> depth;
};
