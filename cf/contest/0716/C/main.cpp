#include <bits/stdc++.h>
using i64 = long long;

using namespace std;

vector<i64> solve() {
  int n;
  cin >> n;

  vector<i64> res(n);
  i64 x = 2;
  for (i64 k = 2; k <= n + 1; ++k) {
    i64 xk = k * (k - 1);
    i64 y = k * xk - x / (k - 1);
    assert(y <= 1e18);
    res[k - 2] = i64(y);
    x = xk;
  }
  return res;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  auto res = solve();
  for (auto x : res) cout << x << '\n';
}
