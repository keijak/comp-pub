#include <bits/stdc++.h>
using namespace std;
using Float = double;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(8);

  int n;
  cin >> n;

  const Float ninv = 1.0 / n;
  const Float c = Float(1 + n) * 0.5 * ninv;
  Float acc = 1;
  Float expectation = 0;
  int m = min(n, 6000);
  for (int k = 1; k <= m; ++k) {
    expectation += acc * k * k;
    acc *= Float(n - k) * ninv;
  }
  expectation *= c;
  cout << expectation << "\n";
}
