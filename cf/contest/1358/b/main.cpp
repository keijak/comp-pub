#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

int solve(int n, vector<i64>& a) {
  sort(a.begin(), a.end());
  for (int i = n - 1; i >= 0; --i) {
    if (a[i] <= i + 1) {
      return i + 2;
    }
  }
  return 1;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;
    vector<i64> a(n);
    for (auto& x : a) cin >> x;
    cout << solve(n, a) << endl;
  }
}
