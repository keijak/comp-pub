#include <bits/stdc++.h>
using i64 = long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
using namespace std;

i64 count_forbidden(i64 n) {
  stringstream ss;
  ss << n;
  string s = ss.str();
  int m = s.size();

  auto dp = vector(m + 1, vector(2, 0LL));
  dp[0][0] = 1;
  for (int i = 0; i < m; ++i) {
    int sd = s[i] - '0';
    REP(isless, 2) REP(d, 10) {
      int isless2 = isless;
      if (not isless and d > sd) continue;
      if (not isless and d < sd) isless2 = 1;
      if (d == 4 or d == 9) continue;
      dp[i + 1][isless2] += dp[i][isless];
    }
  }
  return n - dp[m][0] - dp[m][1];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 a, b;
  cin >> a >> b;
  i64 ans = count_forbidden(b) - count_forbidden(a - 1);
  cout << ans << endl;
}
