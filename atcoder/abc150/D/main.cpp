#include <atcoder/math>
#include <iostream>
#include <vector>

int solve() {
  int n, m;
  std::cin >> n >> m;
  std::vector<long long> a(n), h(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
    h[i] = a[i] / 2;
  }
  auto [y, z] = atcoder::crt(h, a);
  if (z == 0) return 0;
  int k = m / z;
  return (z * k + y <= m) ? (k + 1) : k;
}

int main() { std::cout << solve() << '\n'; }
