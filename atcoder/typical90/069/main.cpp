#include <bits/stdc++.h>

using i64 = long long;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
// using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

auto solve() {
  i64 n, k;
  cin >> n >> k;
  Mint ans = k;
  if (n >= 2) ans *= k - 1;
  if (n >= 3 and k >= 2) ans *= Mint(k - 2).pow(n - 2);
  return ans.val();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
