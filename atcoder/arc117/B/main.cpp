#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

Mint solve() {
  int n;
  cin >> n;
  vector<i64> a(n);
  for (auto &x : a) cin >> x;

  Mint count = 1;
  set<i64> sa(ALL(a));
  sa.insert(0);
  auto it = sa.rbegin();
  i64 x = *it;
  ++it;
  for (; it != sa.rend(); ++it) {
    const i64 y = *it;
    const i64 d = x - y;
    assert(d > 0);
    count *= d + 1;
    x = y;
  }
  return count;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
