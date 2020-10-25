#include <bits/stdc++.h>

#include <atcoder/modint>
using mint = atcoder::modint1000000007;
using namespace std;

template <class T>
struct Factorials {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  Factorials(size_t n) : fact(n + 1), ifact(n + 1) {
    fact[0] = 1;
    for (size_t i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
    ifact[n] = fact[n].inv();
    for (size_t i = n; i >= 1; --i) ifact[i - 1] = ifact[i] * i;
  }

  // Combination
  T C(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, A, B, C, D;
  cin >> N >> A >> B >> C >> D;

  Factorials<mint> fs(N);
  auto dp = vector(N + 1, mint(0));
  dp[0] = 1;
  auto dp2 = dp;
  for (int i = A; i <= B; ++i) {
    for (int j = C; j <= D; ++j) {
      int ij = i * j;
      if (ij > N) break;
      mint r = fs.fact[ij - 1] * fs.ifact[j - 1] * fs.ifact[i].pow(j) * i;
      for (int s = 0; s + ij <= N; ++s) {
        dp[s + ij] += dp2[s] * fs.C(N - s, ij) * r;
      }
    }
    dp2 = dp;
  }
  cout << dp[N].val() << endl;
}
