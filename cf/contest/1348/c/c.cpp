/* Time: 2020-05-01 17:41:48 */

#include <bits/stdc++.h>
#define FOR(i, l, r) for (size_t i = (l); i < (r); ++i)
#define REP(i, n) FOR(i, 0, n)
using namespace std;
typedef long long ll;

void solve(int k, string& s) {
  sort(s.begin(), s.end());
  if (k == 1) {
    cout << s << '\n';
    return;
  }

  int i;
  for (i = 1; i < k; ++i) {
    char c = s[i];
    if (c != s[0]) {
      cout << s[k - 1] << '\n';
      return;
    }
  }
  if (s.size() == k) {
    cout << s[0] << '\n';
    return;
  }

  int trail = s.size() - 1;
  while (trail >= 0 && s[trail] == s[s.size() - 1]) {
    --trail;
  }
  ++trail;
  if (k < trail) {
    cout << s[0] << s.substr(k) << '\n';
    return;
  }

  int rem = s.size() - k;
  int t = rem / k + (rem % k != 0);
  cout << s[0];
  REP(i, t) { cout << s[k]; }
  cout << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int T;
  cin >> T;
  REP(tc, T) {
    int n, k;
    cin >> n >> k;
    cin.ignore();
    string s;
    getline(cin, s);
    solve(k, s);
  }
  return 0;
}
