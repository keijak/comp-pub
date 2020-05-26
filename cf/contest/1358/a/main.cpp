#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

i64 solve(int n, int m) {
  if (n % 2 == 0) {
    return n / 2 * m;
  } else if (m % 2 == 0) {
    return m / 2 * n;
  } else {
    return n / 2 * m + m / 2 + 1;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int n, m;
    cin >> n >> m;
    cout << solve(n, m) << '\n';
  }
}