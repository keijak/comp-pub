#include <bits/stdc++.h>
using namespace std;
#define chmax(x, y) x = max(x, y)

int main() {
  int N, P;
  cin >> N >> P;
  vector<pair<int, int>> snacks(N);
  for (auto& [a, b] : snacks) cin >> a >> b;
  sort(snacks.rbegin(), snacks.rend());
  vector<int> dp(P + 1);
  int ans = 0;
  for (auto [w, value] : snacks) {
    for (int j = P; j >= 0; --j) {
      int value_sum = dp[j] + value;
      if (j + w <= P) chmax(dp[j + w], value_sum);
      chmax(ans, value_sum);
    }
  }
  cout << ans << endl;
}