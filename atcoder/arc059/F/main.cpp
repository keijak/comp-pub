#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)

template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

int solve() {
  int n;
  string s;
  cin >> n >> s;
  int ssz = ssize(s);

  // dp[i][j] := typed i times, j chars remaining
  vector dp(n + 1, vector(n + 1, Mint(0)));
  dp[0][0] = 1;
  REP(i, n) {
    REP(j, n + 1) {
      if (j + 1 <= n) dp[i + 1][j + 1] += dp[i][j] * 2;
      if (j > 0) {
        dp[i + 1][j - 1] += dp[i][j];
      } else if (j == 0) {
        dp[i + 1][j] += dp[i][j];
      }
    }
  }
  Mint ans = dp[n][ssz] / Mint(2).pow(ssz);
  return ans.val();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
