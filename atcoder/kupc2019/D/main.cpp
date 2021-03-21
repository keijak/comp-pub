#include <bits/stdc++.h>

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
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

template <typename T>
T catalan(const Factorials<T> &fs, int k) {
  auto ret = fs.C(2 * k, k);
  if (k > 0) ret -= fs.C(2 * k, k - 1);
  return ret;
}

Mint solve() {
  int n;
  string s;
  cin >> n >> s;
  Factorials<Mint> fs(n * 2);

  int i = 0;
  Mint ans = 1;
  while (i < n) {
    int r = i;
    while (r < n and s[r] == s[i]) ++r;
    ans *= catalan(fs, r - i);
    i = r;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
