#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  i64 C;
  cin >> N >> C;
  vector<i64> xs(N);
  vector<i64> val(N);
  REP(i, N) { cin >> xs[i] >> val[i]; }

  auto solve = [&]() -> i64 {
    vector<i64> al(N + 1), ar(N + 1);
    vector<i64> bl(N + 1), br(N + 1);
    i64 xp = 0;
    REP(i, N) {
      al[i + 1] = al[i] + val[i] - (xs[i] - xp);
      bl[i + 1] = max(bl[i], al[i + 1] - xs[i]);
      xp = xs[i];
    }
    xp = C;
    REP(i, N) {
      int j = N - i - 1;
      ar[j] = ar[j + 1] + val[j] - (xp - xs[j]);
      br[j] = max(br[j + 1], ar[j] - (C - xs[j]));
      xp = xs[j];
    }
    i64 best = 0;
    REP(i, N + 1) {
      i64 t = al[i] + br[i];
      if (best < t) {
        best = t;
      }
      t = ar[i] + bl[i];
      if (best < t) {
        best = t;
      }
    }
    return best;
  };
  cout << solve() << endl;
}