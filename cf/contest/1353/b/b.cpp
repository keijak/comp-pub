/* Time: 2020-05-14 16:41:35 */

#include <algorithm>
#include <iostream>
#include <vector>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

int64 solve(int n, int k, const vector<int>& a, const vector<int>& b) {
  int64 s = 0;
  REP(i, k) {
    if (a.at(i) < b.at(i)) {
      s += b.at(i);
    } else {
      s += a.at(i);
    }
  }
  for (int i = k; i < n; ++i) {
    s += a[i];
  }

  return s;
}

int main() {
  int T;
  cin >> T;

  while (T--) {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    vector<int> b(n);
    int z;
    REP(j, n) {
      cin >> z;
      a[j] = z;
    }
    REP(j, n) {
      cin >> z;
      b[j] = z;
    }
    sort(a.begin(), a.end());
    sort(b.rbegin(), b.rend());
    cout << solve(n, k, a, b) << endl;
  }
  return 0;
}
