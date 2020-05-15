/* Time: 2020-05-14 16:35:28 */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

int64 solve(int64 n, int64 m) {
  if (n == 1) {
    return 0;
  }
  if (n == 2) {
    return m;
  }
  return 2 * m;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int T;
  cin >> T;
  REP(i, T) {
    int64 n, m;
    cin >> n >> m;
    cout << solve(n, m) << endl;
  }
}
