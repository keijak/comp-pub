#include <bits/stdc++.h>

#include <atcoder/segtree>

using namespace std;
using i64 = long long;

i64 sum_op(i64 a, i64 b) { return a + b; }

i64 sum_e() { return 0LL; }

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  atcoder::segtree<i64, sum_op, sum_e> segc(vector<i64>(n + 1)),
      segs(vector<i64>(n + 1));
  for (int i = 0; i < q; ++i) {
    i64 t, x, y, v;
    cin >> t >> x >> y >> v;
    if (t == 0) {
      segc.set(x, 1);
      if (x % 2 == 0) v = -v;
      segs.set(x, v);
    } else if (x == y) {
      cout << v << "\n";
    } else if (x < y) {
      auto count = segc.prod(x, y);
      if (count != y - x) {
        cout << "Ambiguous\n";
      } else {
        auto s = segs.prod(x, y);
        s += (x % 2 == 0) ? v : -v;
        if (y % 2 == 1) s = -s;
        cout << s << "\n";
      }
    } else {
      auto count = segc.prod(y, x);
      if (count != x - y) {
        cout << "Ambiguous\n";
      } else {
        auto s = segs.prod(y, x);
        s += (x % 2 == 0) ? -v : v;
        if (y % 2 == 0) s = -s;
        cout << s << "\n";
      }
    }
  }
}
