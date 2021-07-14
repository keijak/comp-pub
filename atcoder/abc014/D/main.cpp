#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

class LCA {
 public:
  LCA(int n) : n(n), adj(n), parent(K, vector<int>(n, -1)), depth(n, -1) {}

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

  static const int K = 30;
  int n;
  vector<vector<int>> adj;
  vector<vector<int>> parent;
  vector<int> depth;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  LCA lca(n);
  REP(i, n - 1) {
    int x, y;
    cin >> x >> y;
    x--;
    y--;
    lca.add_edge(x, y);
  }
  lca.build();

  int Q;
  cin >> Q;
  REP(i, Q) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    int r = lca.query(a, b);
    int len = depth[a] - depth[r] + depth[b] - depth[r] + 1;
    cout << len << '\n';
  }
}
