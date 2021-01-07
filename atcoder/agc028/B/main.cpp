#include <bits/stdc++.h>

#include <atcoder/modint>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)

using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

int solve() {
  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x : a) cin >> x;

  vector<Mint> cum(n + 1);
  REP(i, 1, n + 1) { cum[i] = cum[i - 1] + Mint(i).inv(); }
  Mint ans = 0;
  REP(i, n) { ans += (cum[i + 1] + cum[n - i] - 1) * a[i]; }
  REP(i, 2, n + 1) { ans *= i; }
  return ans.val();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
