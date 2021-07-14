#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

i64 solve(i64 h, i64 c, i64 t) {
  if (t >= h) return 1;
  i64 m2 = h + c;
  if (2 * t <= m2) return 2;
  auto higher_than_t = [&](i64 x) -> bool {
    return (x + 1) * h + x * c >= t * (2 * x + 1);
  };
  //   auto f = [&](i64 x) -> double {
  //     return (double)((x + 1) * h + x * c) / (double)(2 * x + 1);
  //   };
  i64 ok = 0, ng = 100000000;
  assert(higher_than_t(ok));
  assert(!higher_than_t(ng));
  while (ng - ok > 1) {
    i64 m = (ok + ng) / 2;
    if (higher_than_t(m)) {
      ok = m;
    } else {
      ng = m;
    }
  }

  i64 s1 = ((ok + 1) * h + ok * c) * (2 * ng + 1);
  i64 s2 = ((ng + 1) * h + ng * c) * (2 * ok + 1);
  if (s1 + s2 <= 2 * t * (2 * ok + 1) * (2 * ng + 1)) {
    return ok * 2 + 1;
  } else {
    return ng * 2 + 1;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    i64 h, c, t;
    cin >> h >> c >> t;
    cout << solve(h, c, t) << '\n';
  }
}
