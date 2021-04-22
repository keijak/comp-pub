#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}

using namespace std;

i64 dp[2][5005];

i64 solve() {
  int n;
  cin >> n;
  vector<array<int, 3>> tasks(n);
  int dmax = 0;
  REP(i, n) {
    int d, c, s;
    cin >> d >> c >> s;
    tasks[i] = {d, c, s};
    chmax(dmax, d);
  }
  sort(ALL(tasks));
  const int DMAX = dmax + 5;

  REP(i, n) {
    const auto &[d, c, s] = tasks[i];
    const int lim = d - c + 1;
    for (int j = 1; j <= lim; ++j) {
      chmax(dp[1][j + c], dp[0][j] + s);
    }
    for (int j = 2; j < DMAX; ++j) {
      chmax(dp[1][j], dp[1][j - 1]);
      dp[0][j] = dp[1][j];
    }
  }
  return dp[0][DMAX - 1];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
