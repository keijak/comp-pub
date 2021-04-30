#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using u32 = unsigned;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}
template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
using namespace std;

Mint solve() {
  int n;
  cin >> n;
  vector<u32> a(n), b(n + 1);
  cin >> a;
  REP(i, n) b[i + 1] = b[i] ^ a[i];

  vector dp(n + 1, Mint(0)), dpcum(n + 2, Mint(0));
  dp[0] = 1;
  dpcum[1] = 1;
  map<u32, int> pre;
  REP(i, 1, n + 1) {
    auto it = pre.find(b[i]);
    int j = (it == pre.end()) ? 0 : it->second;
    dp[i] = dpcum[i] - dpcum[j];
    dpcum[i + 1] = dpcum[i] + dp[i];
    pre[b[i]] = i;
  }
  Mint ans = 0;
  REP(i, n) ans += dp[i];
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
