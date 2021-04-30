#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)

using namespace std;

const int L = 1002;
int grid[L][L];

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n;
  cin >> n;
  REP(i, n) {
    int lx, ly, rx, ry;
    cin >> lx >> ly >> rx >> ry;
    ++grid[ly][lx];
    --grid[ly][rx];
    --grid[ry][lx];
    ++grid[ry][rx];
  }
  REP(i, L) {
    REP(j, 1, L) { grid[i][j] += grid[i][j - 1]; }
  }
  REP(j, L) {
    REP(i, 1, L) { grid[i][j] += grid[i - 1][j]; }
  }

  vector<int> ans(n + 1, 0);
  REP(i, L) REP(j, L) {
    const int k = grid[i][j];
    ++ans[k];
  }
  for (int k = 1; k <= n; ++k) {
    cout << ans[k] << "\n";
  }
}
