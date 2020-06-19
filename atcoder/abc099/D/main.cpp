//#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define debug(...)
#define DEBUG(...)
#else
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, C;
  cin >> N >> C;
  vector<vector<int>> D(C, vector<int>(C));
  REP(i, C) REP(j, C) { cin >> D[i][j]; }
  vector<vector<int>> color(N, vector<int>(N));
  vector<vector<int>> color_count(3, vector<int>(C));
  vector<int> cell_count(3);
  REP(i, N) REP(j, N) {
    int c;
    cin >> c;
    c--;
    color[i][j] = c;
    color_count[(i + j) % 3][c]++;
    cell_count[(i + j) % 3]++;
  }
  vector<vector<pair<i64, int>>> min_bad(3);
  REP(i, 3) {
    REP(y, C) {
      i64 dsum = 0;
      REP(x, C) { dsum += color_count[i][x] * D[x][y]; }
      if (min_bad[i].size() < 3) {
        min_bad[i].emplace_back(dsum, y);
        sort(min_bad[i].begin(), min_bad[i].end());
      } else if (dsum < min_bad[i][2].first) {
        min_bad[i][2] = make_pair(dsum, y);
        sort(min_bad[i].begin(), min_bad[i].end());
      }
    }
  }
  i64 ans = 1LL << 50;
  for (auto [d1, c1] : min_bad[0]) {
    for (auto [d2, c2] : min_bad[1]) {
      if (c2 == c1) continue;
      for (auto [d3, c3] : min_bad[2]) {
        if (c3 == c1 || c3 == c2) continue;
        i64 ds = d1 + d2 + d3;
        if (ans > ds) {
          ans = ds;
          DEBUG(ans, c1, c2, c3, d1, d2, d3);
        }
      }
    }
  }
  cout << ans << endl;
}
