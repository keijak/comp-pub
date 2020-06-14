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

const i64 INF = 1LL << 50;
int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 H, W, K;
  cin >> H >> W >> K;
  pair<i64, i64> start, goal;
  cin >> start.first >> start.second;
  cin >> goal.first >> goal.second;
  start.first--;
  start.second--;
  goal.first--;
  goal.second--;
  vector<string> grid(H);
  cin.ignore();
  REP(i, H) {
    getline(cin, grid[i]);
    assert(grid[i].size() == (size_t)W);
  }
  map<tuple<int, int, int>, i64> distm;
  auto get_dist = [&](int r, int c, int dir) -> i64 {
    auto it = distm.find(make_tuple(r, c, dir));
    if (it == distm.end()) return INF;
    return it->second;
  };
  auto solve = [&]() -> i64 {
    priority_queue<tuple<i64, int, int, int>> pq;
    REP(i, 4) { pq.emplace(0LL, start.first, start.second, i); }
    while (pq.size()) {
      auto [d, r, c, dir] = pq.top();
      d *= -1;
      pq.pop();
      if (get_dist(r, c, dir) <= d) continue;
      if (r == goal.first && c == goal.second) {
        return (d + K - 1) / K;
      }
      distm[{r, c, dir}] = d;
      DEBUG(r, c, d);
      REP(j, 4) {
        i64 nd = d;
        if (j > 0 && nd % K != 0) {
          nd = nd - nd % K + K;
        }
        nd++;
        int ndir = (dir + j) % 4;
        int nr = r + dx[ndir];
        int nc = c + dy[ndir];
        if (nr < 0 || nr >= H) continue;
        if (nc < 0 || nc >= W) continue;
        if (grid[nr][nc] == '@') continue;
        if (get_dist(nr, nc, ndir) <= nd) continue;
        pq.emplace(-nd, nr, nc, ndir);
      }
    }
    return -1LL;
  };
  i64 ans = solve();
  cout << ans << endl;
}