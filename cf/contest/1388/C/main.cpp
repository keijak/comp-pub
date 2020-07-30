#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
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

#define chmax(x, y) x = max(x, y)

bool solve() {
  int n, m;
  cin >> n >> m;
  vector<i64> popu(n);
  vector<i64> happ(n);
  REP(i, n) cin >> popu[i];
  REP(i, n) cin >> happ[i];
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int x, y;
    cin >> x >> y;
    x--;
    y--;
    g[x].push_back(y);
    g[y].push_back(x);
  }

  int max_depth = 0;
  vector<int> parent(n), depth(n);
  map<int, vector<int>> depth_to_node;
  auto dfs_par = [&](auto rec, int v, int p, int d) -> void {
    parent[v] = p;
    depth[v] = d;
    depth_to_node[d].push_back(v);
    chmax(max_depth, d);
    for (int u : g[v]) {
      if (u != p) {
        rec(rec, u, v, d + 1);
      }
    }
  };
  dfs_par(dfs_par, 0, -1, 0);

  vector<i64> cgood(n), cbad(n);
  for (int d = max_depth; d >= 0; --d) {
    for (auto v : depth_to_node[d]) {
      i64 c = cgood[v] + cbad[v];  // commuters
      i64 p = popu[v];
      i64 h = happ[v];
      if ((p + c + h) % 2 == 1 || (p + c - h) % 2 == 1 || p + c - h < 0) {
        return false;
      }
      i64 num_good = (p + c + h) / 2;
      i64 num_bad = (p + c - h) / 2;
      if (num_good < cgood[v]) return false;
      int par = parent[v];
      if (par >= 0) {
        cgood[par] += num_good;
        cbad[par] += num_bad;
      } else {
        assert(v == 0);
        if (p + c != m) return false;
      }
    }
  }
  return true;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    cout << (solve() ? "YES\n" : "NO\n");
  }
}
