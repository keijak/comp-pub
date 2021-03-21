#include <bits/stdc++.h>
using i64 = long long;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

Mint solve() {
  i64 n, k;
  cin >> n >> k;
  vector<Mint> counts(k + 1, 0);
  Mint ans = 0;
  for (i64 x = k; x > 0; --x) {
    i64 m = k / x;  // multiples
    Mint c = Mint(m).pow(n);
    for (i64 i = 2 * x; i <= k; i += x) {
      c -= counts[i];
    }
    ans += x * c;
    counts[x] = c;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
