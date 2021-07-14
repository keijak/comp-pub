#define MY_DEBUG 0
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if MY_DEBUG
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

i64 solve() {
  int n;
  cin >> n;
  vector<i64> a(n);
  for (auto& x : a) cin >> x;

  if (n == 1) {
    return a[0];
  }

  vector<vector<i64>> dp(n + 1, vector<i64>(3));
  dp[0][0] = a[0];
  dp[0][1] = 0;
  dp[0][2] = a[0];
  for (int i = 1; i <= n; ++i) {
    if (i % 2 == 1) {
      dp[i][0] = dp[i - 1][0];
      dp[i][2] = dp[i - 1][2];
      dp[i][1] = dp[i - 1][1] + (i < n ? a[i] : 0LL);
    } else {
      dp[i][0] = dp[i - 1][0] + (i < n ? a[i] : 0LL);
      dp[i][2] = max(dp[i - 1][2], dp[i - 1][1]) + (i < n ? a[i] : 0LL);
      dp[i][1] = max(dp[i - 1][1], dp[i - 1][0]);
    }
  }

  vector<vector<i64>> dp2(n + 1, vector<i64>(3));
  dp2[0][0] = a[0];
  dp2[0][1] = 0;
  dp2[0][2] = a[0];
  for (int i = 1; i <= n; ++i) {
    if (i % 2 == 1) {
      dp2[i][0] = dp2[i - 1][0];
      dp2[i][2] = max(dp2[i - 1][2], dp2[i - 1][1]);
      dp2[i][1] = max(dp2[i - 1][1], dp2[i - 1][0]) + (i < n ? a[i] : 0LL);
    } else {
      dp2[i][0] = dp2[i - 1][0] + (i < n ? a[i] : 0LL);
      dp2[i][2] = dp2[i - 1][2] + (i < n ? a[i] : 0LL);
      dp2[i][1] = dp2[i - 1][1];
    }
  }

  /*
    cerr << "============" << endl;
    REP(i, 3) {
      REP(j, n + 1) { cerr << dp2[j][i] << '\t'; }
      cerr << endl;
    }
    cerr << "-----------" << endl;
    REP(i, 3) {
      REP(j, n + 1) { cerr << dp[j][i] << '\t'; }
      cerr << endl;
    }
    cerr << "============" << endl;
  */
  i64 ans = dp[n][2];
  i64 ans2 = dp2[n][2];
  if (ans < ans2) {
    ans = ans2;
  } else {
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    cout << solve() << '\n';
  }
}
