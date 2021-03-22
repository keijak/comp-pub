#include <bits/stdc++.h>

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

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
  T C(int n, int k) const {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }
};

Mint solve() {
  int n, m;
  cin >> n >> m;

  if (n < 3) {
    return 0;
  }

  Factorials fs(n + m);
  Mint total = fs.C(n + m - 2, n - 1);

  // Coefficient of x^k in (1 + x + x^2)^(m-1)
  // 多項定理による
  auto f = [&](int k) -> Mint {
    Mint coef = 0;
    for (int b = 0; b <= m - 1; ++b) {
      int a = k - 2 * b;
      if (a < 0) break;
      int c = m - 1 - a - b;
      if (c < 0) continue;
      coef += fs.fact[m - 1] * fs.ifact[a] * fs.ifact[b] * fs.ifact[c];
    }
    return coef;
  };

  // d = [x^(n-1)] g(x)
  // where g(x) = (1 + x) * (1 + x + x^2)^(m - 1)
  Mint d = f(n - 1) + f(n - 2);
  return total - d;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
