#include <bits/stdc++.h>

#include <atcoder/math>

using i64 = long long;
using namespace std;

i64 solve() {
  i64 n, s, k;
  cin >> n >> s >> k;

  i64 q = n - s;
  i64 g = std::gcd(n, k);
  if (q % g != 0) return -1;
  q /= g, n /= g, k /= g;
  return atcoder::inv_mod(k, n) * q % n;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) cout << solve() << "\n";
}
