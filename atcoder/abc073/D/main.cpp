#define ENABLE_DEBUG 1
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
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

const int INF = 100000000;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M, R;
  cin >> N >> M >> R;
  vector<int> checkpoints(R);
  REP(i, R) {
    int x;
    cin >> x;
    checkpoints[i] = x - 1;
  }
  vector<vector<int>> dist(N, vector<int>(N, INF));
  REP(i, M) {
    int a, b, c;
    cin >> a >> b >> c;
    a--;
    b--;
    dist[a][b] = dist[b][a] = c;
  }
  REP(k, N) REP(i, N) REP(j, N) {
    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
  }

  // REP(i, R) REP(j, i) { DEBUG(j, i, dist[checkpoints[i]][checkpoints[j]]); }
  const int L = 1 << R;
  vector<vector<int>> dp(L, vector<int>(R, INF));

  auto solve = [&](auto rec, int unvisited, int cur) -> i64 {
    if (!unvisited) return 0LL;
    if (dp[unvisited][cur] != INF) return dp[unvisited][cur];
    const int cc = checkpoints[cur];
    i64 res = INF;
    REP(i, R) {
      if (unvisited & (1 << i)) {
        int nxt = checkpoints[i];
        int state = unvisited & ~(1 << i);
        res = min(res, dist[cc][nxt] + rec(rec, state, i));
      }
    }
    dp[unvisited][cur] = res;
    return res;
  };
  i64 ans = INF;
  REP(i, R) {
    int state = (L - 1) & ~(1 << i);
    ans = min(ans, solve(solve, state, i));
  }
  cout << ans << '\n';
}