#include <bits/stdc++.h>
using i64 = long long;

using namespace std;

optional<tuple<i64, i64, i64>> solve() {
  i64 n, p, w, d;
  cin >> n >> p >> w >> d;

  auto g = gcd(w, d);
  if (p % g != 0) return nullopt;
  p /= g;
  w /= g;
  d /= g;

  for (i64 y = 0; y < w; ++y) {
    i64 s = p - y * d;
    if (s >= 0 and s % w == 0) {
      i64 x = s / w;
      if (x + y <= n) {
        return tuple(x, y, n - x - y);
      }
    }
  }
  return nullopt;
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  auto res = solve();
  if (not res.has_value()) {
    cout << -1 << endl;
  } else {
    auto [x, y, z] = res.value();
    cout << x << ' ' << y << ' ' << z << endl;
  }
}
