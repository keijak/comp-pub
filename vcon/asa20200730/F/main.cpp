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
  int N, P;
  cin >> N >> P;
  vector<tuple<int, int>> snacks(N);
  REP(i, N) {
    int a, b;
    cin >> a >> b;
    snacks[i] = {a, b};
  }
  sort(snacks.rbegin(), snacks.rend());
  vector<vector<i64>> dp(N + 1, vector<i64>(P + 1));
  i64 ans = 0;
  for (int i = 1; i <= N; ++i) {
    auto [w, value] = snacks[i - 1];
    for (int j = 0; j <= P; ++j) {
      dp[i][j] = dp[i - 1][j];
    }
    for (int j = 0; j <= P; ++j) {
      if (ans < dp[i - 1][j] + value) {
        ans = dp[i - 1][j] + value;
        DEBUG(ans, value);
      }
      if (j + w <= P) {
        dp[i][j + w] = max(dp[i][j + w], dp[i - 1][j] + value);
      }
    }
  }
  cout << ans << endl;
}