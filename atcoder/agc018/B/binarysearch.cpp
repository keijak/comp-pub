#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

using namespace std;

i64 solve() {
  int n, m;
  cin >> n >> m;
  auto A = vector(n, vector(m, 0));
  REP(i, n) REP(j, m) {
    int sp;
    cin >> sp;
    A[i][j] = --sp;
  }

  auto check = [&](i64 x) -> bool {
    vector<deque<int>> q(n);
    REP(i, n) { q[i].assign(ALL(A[i])); }

    vector<bool> avail(m, true);
    vector<int> count;
    while (true) {
      count.assign(m, 0);
      int max_count = 0;
      REP(i, n) {
        int sp = q[i].front();
        assert(avail[sp]);
        max_count = max(max_count, ++count[sp]);
      }
      if (max_count <= x) return true;
      REP(i, m) {
        if (count[i] > x) avail[i] = false;
      }

      REP(i, n) {
        while (not q[i].empty()) {
          int sp = q[i].front();
          if (avail[sp]) break;
          q[i].pop_front();
        }
        if (q[i].empty()) return false;
      }
    }
  };

  i64 fv = 0, tv = n + 1;
  while (tv - fv > 1) {
    i64 mid = (tv + fv) / 2;
    if (check(mid)) {
      tv = mid;
    } else {
      fv = mid;
    }
  }
  return tv;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
