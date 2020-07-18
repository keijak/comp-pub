#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
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
  int H, W;
  cin >> H >> W;
  vector<string> grid(H);
  REP(i, H) { cin >> grid[i]; }

  auto bfs = [&](int r, int c) -> tuple<int, int, int> {
    vector<int> dx = {0, 0, 1, -1};
    vector<int> dy = {1, -1, 0, 0};
    vector<string> g = grid;
    deque<tuple<int, int, int>> q;
    int maxd = -1;
    int mr = -1, mc = -1;
    q.emplace_back(r, c, 0);
    g[r][c] = 'x';
    while (q.size()) {
      auto [r1, c1, d1] = q.front();
      q.pop_front();
      if (maxd < d1) {
        maxd = d1;
        mr = r1;
        mc = c1;
      }
      REP(i, 4) {
        int r2 = r1 + dx[i];
        int c2 = c1 + dy[i];
        if (r2 < 0 || r2 >= H || c2 < 0 || c2 >= W) continue;
        if (g[r2][c2] != '.') continue;
        g[r2][c2] = 'x';
        q.emplace_back(r2, c2, d1 + 1);
      }
    }
    return {mr, mc, maxd};
  };
  int maxd = 0;
  REP(r, H) REP(c, W) {
    if (grid[r][c] == '.') {
      auto [r1, c1, d1] = bfs(r, c);
      if (maxd < d1) maxd = d1;
    }
  }
  cout << maxd << endl;
}
