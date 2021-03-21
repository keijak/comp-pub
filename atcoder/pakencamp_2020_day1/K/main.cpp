#include <iostream>
using namespace std;

int a[302], acc[302], ans[302];

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n;
  cin >> n;
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
    acc[i + 1] = acc[i] + a[i];
  }
  const int total = acc[n];
  for (int i = 1; i * i <= total; ++i) {
    const auto &[quot, rem] = std::div(total, i);
    if (rem != 0) continue;
    for (int j = 0; j <= 1; ++j) {
      int y;
      if (j == 0) {
        y = i;
      } else if (quot != i) {
        y = quot;
      } else {
        break;
      }
      int cnt = 0;
      for (int k = 1; k <= n; ++k) {
        if (acc[k] % y == 0) {
          ++cnt;
          if (ans[cnt] < y) ans[cnt] = y;
        }
      }
    }
  }
  for (int *x = ans, *e = ans + n; x != e;) {
    cout << *++x << "\n";
  }
}
