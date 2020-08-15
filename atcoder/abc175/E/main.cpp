#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int R, C, K;
  cin >> R >> C >> K;
  V<V<int>> grid(R, V<int>(C));
  REP(i, K) {
    int r, c, v;
    cin >> r >> c >> v;
    r--;
    c--;
    grid[r][c] = v;
  }

  V<V<V<i64>>> dp(R + 1, V<V<i64>>(C + 1, V<i64>(4)));
  for (int r = 1; r <= R; ++r) {
    for (int c = 1; c <= C; ++c) {
      i64 prow = 0;
      REP(s, 4) { chmax(prow, dp[r - 1][c][s]); }
      dp[r][c][0] = max(prow, dp[r][c - 1][0]);
      dp[r][c][1] = prow + grid[r - 1][c - 1];
      for (int s = 1; s <= 3; ++s) {
        chmax(dp[r][c][s], dp[r][c - 1][s]);
        chmax(dp[r][c][s], dp[r][c - 1][s - 1] + grid[r - 1][c - 1]);
      }
    }
  }

  i64 ans = 0;
  REP(s, 4) { chmax(ans, dp[R][C][s]); }
  cout << ans << endl;
}
