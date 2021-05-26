#include <bits/stdc++.h>

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

auto solve() {
  int N, L;
  cin >> N >> L;
  auto dp = vector(N + 1, Mint(0));
  dp[0] = 1;
  for (int i = 1; i <= N; ++i) {
    dp[i] = dp[i - 1] + (i - L >= 0 ? dp[i - L] : 0);
  }
  return dp[N];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
