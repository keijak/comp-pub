#include <bits/stdc++.h>

#include <atcoder/math>

using i64 = long long;
using namespace std;

i64 solve() {
  i64 n, s, k;
  cin >> n >> s >> k;

  auto [y, z] = atcoder::crt({n - s, 0}, {n, k});
  if (y == 0 and z == 0) return -1;
  return y / k;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) cout << solve() << "\n";
}
