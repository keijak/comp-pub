#include <bits/stdc++.h>
using namespace std;

#define forr(n) for (int i = 0; i < n; i++)

void solve() {
  string s;
  cin >> s;
  int n = s.size();
  vector<int> occ(26, 0);
  vector<bool> pres(26, false);
  for (int i = 0; i < n; i++) {
    occ[s[i] - 'a']++;
    pres[s[i] - 'a'] = true;
  }
  int f = 0, b = n - 1;
  bool ans = true;
  while (f <= b) {
    int mi = int(1e9) + 7;
    int mx = 0;
    for (int i = 0; i < 26; i++) {
      if (pres[i]) mi = min(mi, occ[i]);
      mx = max(mx, occ[i]);
    }
    if (abs(mi - mx) > 1) ans = false;
    if (mi < mx and (occ[s[f] - 'a'] == mi or occ[s[b] - 'a'] == mi))
      ans = false;
    occ[s[f] - 'a']--;
    occ[s[b] - 'a']--;
    f++;
    b--;
  }
  cout << (ans ? "YES\n" : "NO\n");
}

int main() {
  int t;
  cin >> t;
  while (t--) solve();
}
