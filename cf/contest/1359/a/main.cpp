#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

i64 solve(int n, int m, int k) {
  int cards = n / k;
  if (m <= cards) {
    return m;
  }
  int r = (m - cards);
  return cards - (r + (k - 2)) / (k - 1);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    int n, m, k;
    cin >> n >> m >> k;
    cout << solve(n, m, k) << endl;
  }
}