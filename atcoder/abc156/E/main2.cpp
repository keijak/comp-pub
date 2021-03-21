#include <bits/stdc++.h>
using i64 = long long;

using namespace std;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template <class T = Mint>
struct Factorials {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  // n: max cached value.
  Factorials(int n) : fact(n + 1), ifact(n + 1) {
    assert(n > 0 and n < T::mod());
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) ifact[i - 1] = ifact[i] * i;
  }

  // Combination (nCk)
  T C(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }
};

Mint solve() {
  i64 N, K;
  cin >> N >> K;
  if (K >= N - 1) {
    K = N - 1;
  }
  Factorials fs(N);

  Mint ans = 1;        // f(0);
  ans += N * (N - 1);  // f(1)
  for (i64 i = 2; i <= K; ++i) {
    Mint movers = fs.C(N, i);  // choose movers.
    Mint separators = fs.C(N - 1, N - i - 1);
    ans += movers * separators;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
