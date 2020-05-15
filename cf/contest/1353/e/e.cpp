/* Time: 2020-05-14 18:01:50 */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

int64 solve(int64 n, int64 k, string& s) {
  int first = -1, last = -1;
  REP(i, n) {
    if (s[i] == '1') {
      first = i;
      break;
    }
  }
  for (int i = n - 1; i >= 0; --i) {
    if (s[i] == '1') {
      last = i;
      break;
    }
  }
  if (first < 0 || last < 0) {
    return 0;
  }
  if (first == last) {
    return 0;
  }

  vector<int> ones(k, 0);
  int64 tot = 0;
  for (int i = 0; i < n; ++i) {
    int m = i % k;
    if (s[i] == '1') {
      tot++;
      ones[m]++;
    }
  }
  int64 mind = 10000000;
  REP(i, k) {
    int64 tself = 0;
    int m = (n + n % k) / k;
    vector<int64> dp(m, n);
    REP(i, m) {
      

    }

        int64 ton = cnts[i] - ones[i];
    int64 tother = tot - ones[i];
    mind = min(mind, tself + tother);
  }
  return mind;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int T;
  cin >> T;
  while (T--) {
    int64 n, k;
    cin >> n >> k;
    cin.ignore();
    string s;
    getline(cin, s);
    // cout << "ok:" << s << endl;
    assert(s.size() == n);
    cout << solve(n, k, s) << endl;
  }
}
