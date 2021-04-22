#include <bits/stdc++.h>

#include <atcoder/fenwicktree>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

using namespace std;

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> segs(n);
  REP(i, m) {
    int l, r;
    cin >> l >> r;
    --l, --r;
    if (l > r) swap(l, r);
    segs[l].push_back(r);
  }

  atcoder::fenwick_tree<i64> fw(n);
  i64 ans = 0;
  REP(l, n) {
    for (auto r : segs[l]) {
      ans += fw.sum(l + 1, r);
    }
    for (auto r : segs[l]) {
      fw.add(r, 1);
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
