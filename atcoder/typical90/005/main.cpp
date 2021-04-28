#include <bits/stdc++.h>

#include <atcoder/math>
#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
using i64 = long long;

using namespace std;

Mint solve() {
  i64 N, B, K;
  cin >> N >> B >> K;
  vector<int> C(K);
  REP(i, K) cin >> C[i];

  auto f = [&](auto &f, i64 x) -> vector<Mint> {
    vector<Mint> res(B);
    if (x == 1) {
      for (int x : C) ++res[x % B];
      return res;
    }
    if (x & 1) {
      auto dp = f(f, x - 1);
      REP(i, B) {
        for (int x : C) {
          int j = (i * 10 + x) % B;
          res[j] += dp[i];
        }
      }
      return res;
    }
    auto dp = f(f, x / 2);
    const i64 D = atcoder::pow_mod(10, x / 2, B);
    REP(i, B) REP(j, B) {
      int nex = (i * D + j) % B;
      res[nex] += dp[i] * dp[j];
    }
    return res;
  };

  auto ans = f(f, N);
  return ans[0];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
