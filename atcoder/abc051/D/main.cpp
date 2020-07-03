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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  const int INF = 1 << 25;
  vector<vector<int>> G(N, vector<int>(N, INF));
  REP(i, M) {
    int a, b, c;
    cin >> a >> b >> c;
    a--;
    b--;
    G[a][b] = c;
    G[b][a] = c;
  }
  vector<vector<int>> dist = G;
  REP(k, N) REP(i, N) REP(j, N) {
    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
  }
  int ans = 0;
  REP(i, N) REP(j, i) {
    if (G[i][j] != INF && G[i][j] > dist[i][j]) {
      ans++;
    }
  }
  cout << ans << endl;
}