#include <bits/stdc++.h>
using i64 = long long;
using Float = long double;
using namespace std;

Float solve() {
  i64 x, p;
  cin >> x >> p;

  const bool odd_x = x & 1;
  if (p == 100) {
    i64 dist = odd_x ? x + 1 : x;
    return Float(dist / 2);
  }

  Float pp = Float(p) / 100;
  if (odd_x) {
    return 1.0 + (x - 1) / 2 + (1 - pp) * (x + 1) / 2 / pp;
  } else {
    return (x / 2) / pp;
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(20);
  cout << solve() << "\n";
}
