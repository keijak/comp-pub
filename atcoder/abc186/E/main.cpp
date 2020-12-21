#include <bits/stdc++.h>
using i64 = long long;

// Returns floor(x / y).
i64 floordiv(i64 x, i64 y) {
  int sign = (x < 0 ? -1 : 1) * (y < 0 ? -1 : 1);
  if (sign == 1) {
    return std::abs(x) / std::abs(y);
  } else {
    auto d = std::abs(y);
    return -((std::abs(x) + d - 1) / d);
  }
}

// Etended Euclidean algorithm.
// Returns [g, x, y] where g = a*x + b*y = GCD(a, b).
// Either x or y is negative.
std::array<i64, 3> ext_gcd(i64 a, i64 b) {
  i64 ax = 1, ay = 0, bx = 0, by = 1;
  while (b != 0) {
    auto r = a % b;
    auto q = a / b;
    a = b;
    b = r;
    ax -= bx * q;
    std::swap(ax, bx);
    ay -= by * q;
    std::swap(ay, by);
  }
  return {a, ax, ay};
}

// Linear Diophantine equation (Bezout equation).
// Solves a*x + b*y = c (a,b,c: positive integers).
// Returns [x, y] where 0 <= x < b/gcd(a,b).
// If you want y which satisfies "0 <= y < a/gcd(a,b)", swap arguments:
// i.e. invoke `linear_diophantine(b, a, c)`.
std::optional<std::array<i64, 2>> linear_diophantine(i64 a, i64 b, i64 c) {
  auto [g, x, y] = ext_gcd(a, b);
  if (c % g != 0) return std::nullopt;
  a /= g, b /= g, c /= g;
  x *= c, y *= c;
  auto m = floordiv(x, b);
  x -= m * b;
  y += m * a;
  assert(0 <= x and x < b);
  return std::array{x, y};
}

using namespace std;

i64 solve() {
  i64 n, s, k;
  cin >> n >> s >> k;
  auto res = linear_diophantine(k, n, n - s);
  if (!res) return -1;
  return (*res)[0];
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) cout << solve() << "\n";
}
