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

const i64 INF = 1LL << 50;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  vector<i64> costs(M);
  vector<int> keys(M);
  REP(i, M) {
    int b;
    cin >> costs[i] >> b;
    REP(j, b) {
      int c;
      cin >> c;
      --c;
      keys[i] |= 1 << c;
    }
  }

  const int L = 1LL << N;
  vector<vector<i64>> dp(2, vector<i64>(L, INF));
  dp[0][0] = 0;
  REP(i, M) {
    i64 cost = costs[i];
    int key = keys[i];
    dp[1] = dp[0];
    REP(state, L) {
      if (dp[1][state] >= INF) continue;
      dp[0][state | key] = min(dp[0][state | key], dp[1][state] + cost);
    }
  }
  int keyall = 0;
  REP(i, N) { keyall |= 1 << i; }
  i64 ans = dp[0][keyall];
  cout << (ans == INF ? -1 : ans) << endl;
}
