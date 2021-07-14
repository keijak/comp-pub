#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<i64> x1(3), x2(3);
  cin >> x1[0] >> x1[1] >> x1[2] >> x2[0] >> x2[1] >> x2[2];
  vector<i64> dp(N + 1);
  dp[0] = N;
  REP(i, 3) {
    i64 t1 = x1[i], t2 = x2[i];
    if (t1 >= t2) continue;
    for (int j = t1; j <= N; ++j) {
      if (dp[j - t1] >= t1) {
        dp[j] = max(dp[j], dp[j - t1] - t1 + t2);
      }
    }
  }
  i64 donb = *max_element(dp.begin(), dp.end());
  dp.resize(donb + 1);
  fill(dp.begin(), dp.end(), 0);
  dp[0] = donb;
  REP(i, 3) {
    i64 t1 = x1[i], t2 = x2[i];
    if (t1 <= t2) continue;
    for (int j = t2; j <= donb; ++j) {
      if (dp[j - t2] >= t2) {
        dp[j] = max(dp[j], dp[j - t2] - t2 + t1);
      }
    }
  }
  i64 ans = *max_element(dp.begin(), dp.end());
  cout << ans << endl;
}
