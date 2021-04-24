#pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")

#include <atcoder/modint>
#include <iostream>
#include <string>
using Mint = atcoder::modint1000000007;

using namespace std;

const char *T = "atcoder";
Mint dp[2][8];

unsigned solve() {
  int n;
  string s;
  cin >> n >> s;
  Mint *din = dp[0], *dout = dp[1];
  din[0] = 1;
  for (int i = 0; i < n; ++i) {
    const char ch = s[i];
    for (int j = 0; j <= 7; ++j) {
      dout[j] += din[j];
      if (j < 7 and ch == T[j]) {
        dout[j + 1] += din[j];
      }
    }
    swap(din, dout);
    fill(dout, dout + 8, 0);
  }
  return din[7].val();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
