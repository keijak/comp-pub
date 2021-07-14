#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, C;
  cin >> N >> C;
  vector<vector<int>> D(C, vector<int>(C));
  vector<vector<int>> colors(N, vector<int>(N));
  REP(i, C) REP(j, C) { cin >> D[i][j]; }
  REP(i, N) REP(j, N) {
    cin >> colors[i][j];
    colors[i][j]--;
  }

  vector<vector<int>> color_count(3, vector<int>(C));
  REP(r, N) REP(c, N) {
    int k = (r + c) % 3;
    color_count[k][colors[r][c]]++;
  }

  i64 ans = 1LL << 60;
  REP(x, C) {
    i64 cost0 = 0;
    REP(from, C) { cost0 += D[from][x] * color_count[0][from]; }
    REP(y, C) {
      if (x == y) continue;
      i64 cost1 = 0;
      REP(from, C) { cost1 += D[from][y] * color_count[1][from]; }
      REP(z, C) {
        if (z == x || z == y) continue;
        i64 cost2 = 0;
        REP(from, C) { cost2 += D[from][z] * color_count[2][from]; }
        ans = min(ans, cost0 + cost1 + cost2);
      }
    }
  }
  cout << ans << endl;
}
