#include <array>
#include <iostream>
#include <numeric>
#include <optional>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T, typename U>
inline bool chmin(T& a, U b) {
  return a > b and ((a = std::move(b)), true);
}

using namespace std;

// Returns floor(x / y).
i64 floordiv(i64 x, i64 y) {
  if (y <= 0) {
    y *= -1;
    x *= -1;
  }
  if (x >= 0) {
    return x / y;
  } else {
    return -((-x + y - 1) / y);
  }
}

// Etended Euclidean algorithm.
// Returns [g, x, y] where g = a*x + b*y = GCD(a, b).
// Note that g, x, y can be negative.
std::array<i64, 3> ext_gcd(i64 a, i64 b) {
  if (b == 0) return {a, 1LL, 0LL};
  auto res = ext_gcd(b, a % b);  // = (g, x, y)
  res[1] -= (a / b) * res[2];
  std::swap(res[1], res[2]);
  return res;  // = (g, y, x - (a/b)*y)
}

// Linear Diophantine equation (Bezout equation).
// Solves a*x + b*y = c (a,b,c: positive integers).
// Returns [x, y] where 0 <= x < b/gcd(a,b).
std::optional<std::array<i64, 2>> linear_diophantine(i64 a, i64 b, i64 c) {
  auto [g, x, y] = ext_gcd(a, b);
  a /= g, b /= g, c /= g;
  x *= c, y *= c;
  auto m = floordiv(x, b);
  x -= m * b;
  y += m * a;
  return std::array{x, y};
}

const int L = 10000;

i64 solve() {
  int n;
  array<int, 3> data;
  cin >> n >> data[0] >> data[1] >> data[2];
  sort(ALL(data));
  i64 a = data[0], b = data[1], c = data[2];
  i64 g = gcd(a, b);
  i64 ans = L;
  for (i64 z = n / c; z >= 0; --z) {
    const i64 r = n - z * c;
    if ((ans - z) * b < r) break;
    if (r % g != 0) continue;
    const auto res = linear_diophantine(a, b, r);
    if (!res) continue;
    const auto& [x, y] = *res;
    if (y < 0) continue;
    chmin(ans, x + y + z);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
