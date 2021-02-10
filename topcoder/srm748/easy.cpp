#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

const int MOD = int(1e9) + 7;

int gcd(int x, int y) {
  if (x < y) swap(x, y);
  if (y == 0) return x;
  return gcd(y, x % y);
}

struct EraseToGCD {
  int countWays(vector<int> S, int goal) {
    const int n = (int)S.size();
    const int L = 1005;
    vector<int> dp(L), dp_temp(L);
    for (int i = 0; i < n; ++i) {
      if (S[i] % goal != 0) continue;
      int x = S[i];
      dp_temp = dp;
      (dp_temp[x] += 1) %= MOD;
      for (int j = 1; j < L; ++j) {
        int g = gcd(x, j);
        (dp_temp[g] += dp[j]) %= MOD;
      }
      swap(dp_temp, dp);
    }
    return dp[goal];
  }
};

int main() {
  EraseToGCD s;
  int ans;
  // ans = s.countWays({6, 4, 30, 90, 66}, 2);
  // ans = s.countWays({8, 8, 8}, 4);
  ans = s.countWays({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1);
  // ans = s.countWays({2, 2, 2, 2, 2}, 2);
  cout << ans << endl;
}
