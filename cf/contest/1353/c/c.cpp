/* Time: 2020-05-14 17:06:50 */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

int64 solve(int64 n) {
  int64 ans = 0;
  int64 k = n / 2;
  for (int64 i = 1; i <= k; ++i) {
    ans += i * ((2 * i - 1) * 4 + 4);
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int T;
  cin >> T;
  while (T--) {
    int64 n;
    cin >> n;
    cout << solve(n) << endl;
  }
}
