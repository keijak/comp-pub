#define ENABLE_DEBUG 0
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if ENABLE_DEBUG
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                \
  do {                            \
    cerr << " \033[33m (L";       \
    cerr << __LINE__ << ") ";     \
    cerr << #__VA_ARGS__ << ": "; \
    cerr << "\033[0m";            \
    debug(__VA_ARGS__);           \
    cerr << endl;                 \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

const i64 INF = 1LL << 50;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  cin.ignore();
  vector<vector<i64>> adj(N, vector<i64>(N, INF));
  REP(i, N) {
    string s;
    getline(cin, s);
    REP(j, N) {
      if (s[j] == '1') {
        adj[i][j] = 1;
      } else if (j == i) {
        adj[i][j] = 0;
      }
    }
  }

  vector<int> color(N, -1);
  auto dfs = [&](auto rec, int v, int p, int c) -> bool {
    if (color[v] != -1) {
      return (color[v] == c);
    }
    color[v] = c;
    REP(x, N) {
      if (x == v || x == p || adj[v][x] != 1) continue;
      if (!rec(rec, x, v, c ^ 1)) return false;
    }
    return true;
  };

  if (!dfs(dfs, 0, -1, 0)) {
    cout << -1 << endl;
    return 0;
  }

  REP(k, N) REP(i, N) REP(j, N) {
    adj[i][j] = min(adj[i][j], adj[i][k] + adj[k][j]);
  }

  i64 diam = 1;
  REP(i, N) REP(j, N) {
    if (adj[i][j] != INF && diam < adj[i][j]) {
      diam = adj[i][j];
      DEBUG(i, j, diam);
    }
  }
  cout << (diam + 1) << endl;
}
