#include <bits/stdc++.h>
using i64 = long long;
using namespace std;

// Returns x mod y.
i64 floormod(i64 x, i64 y) {
  assert(y > 0);
  auto r = ((x % y) + y) % y;
  assert(0 <= r and r < y);
  return r;
}

// Etended Euclidean algorithm.
// Returns [g, x, y] where g = a*x + b*y = GCD(a, b).
// Either x or y is negative.
std::array<i64, 3> ext_gcd(i64 a, i64 b) {
  if (b == 0) return {a, 1, 0};
  auto res = ext_gcd(b, a % b);  // = (g, x, y)
  res[1] -= (a / b) * res[2];
  std::swap(res[1], res[2]);
  return res;  // = (g, y, x - (a/b)*y)
}

i64 solve() {
  i64 n, s, k;
  cin >> n >> s >> k;

  i64 q = n - s;
  auto [g, x, y] = ext_gcd(n, k);
  if (q % g != 0) return -1;
  q /= g;
  n /= g;
  k /= g;
  return floormod(y, n) * q % n;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) cout << solve() << "\n";
}
