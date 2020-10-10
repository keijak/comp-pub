#include <bits/stdc++.h>

#include <atcoder/modint>

using namespace std;
using mint = atcoder::modint1000000007;

mint solve() {
  long long N, A, B;
  cin >> N >> A >> B;
  if (A + B > N) return 0;
  mint n = N, a = A, b = B;
  mint s = (n - b - a + 1) * (n - b - a + 2) / 2;  // Σk [k=1 to n-b-a+1]
  mint r = (n - a + 1) * (n - b + 1) * s;
  mint t = s * s;
  return (r - t) * 4;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    cout << solve().val() << '\n';
  }
}
