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
  int H, W;
  cin >> H >> W;
  vector<string> grid(H);
  int init_walls = 0;
  REP(i, H) {
    cin >> grid[i];
    assert(grid[i].size() == size_t(W));
    REP(j, W) {
      if (grid[i][j] == '#') {
        init_walls++;
      }
    }
  }
  vector<vector<bool>> visited(H, vector<bool>(W));
  deque<tuple<int, int, int>> q;
  q.emplace_back(0, 0, 1);
  visited[0][0] = true;
  while (q.size()) {
    vector<int> dx = {1, -1, 0, 0};
    vector<int> dy = {0, 0, 1, -1};
    auto [r, c, d] = q.front();
    q.pop_front();
    if (r == H - 1 && c == W - 1) {
      int ans = H * W - d - init_walls;
      cout << ans << endl;
      return 0;
    }
    REP(i, 4) {
      int r2 = r + dx[i];
      int c2 = c + dy[i];
      if (r2 < 0 || r2 >= H || c2 < 0 || c2 >= W) continue;
      if (grid[r2][c2] == '#') continue;
      if (visited[r2][c2]) continue;
      visited[r2][c2] = true;
      q.emplace_back(r2, c2, d + 1);
    }
  }
  cout << -1 << endl;
}
