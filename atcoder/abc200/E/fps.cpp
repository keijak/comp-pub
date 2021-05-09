#include <bits/stdc++.h>

#include <atcoder/convolution>

using i64 = long long;

using namespace std;

int main() {
  i64 n, k;
  cin >> n >> k;

  vector<i64> f1(n + 1, 1);  // = x + x^2 + ... + x^n
  f1[0] = 0;
  auto f2 = atcoder::convolution_ll(f1, f1);
  auto f3 = atcoder::convolution_ll(f2, move(f1));

  i64 rem = k;
  i64 s, x;
  for (s = 3; rem > f3[s]; ++s) {
    rem -= f3[s];
  }
  for (x = max(s - n * 2, 1LL); rem > f2[s - x]; ++x) {
    rem -= f2[s - x];
  }
  const i64 y = max(s - x - n, 1LL) + rem - 1;
  const i64 z = s - x - y;
  cout << x << " " << y << " " << z << endl;
}
