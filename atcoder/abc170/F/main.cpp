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
int rev[4] = {1, 0, 3, 2};

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

  vector<vector<i64>> distm(H, vector<i64>(W, INF));

  auto solve = [&]() -> i64 {
    deque<tuple<i64, int, int, int>> pq;
    pq.emplace_back(0LL, start.first, start.second, -1);
    distm[start.first][start.second] = 0;
    while (pq.size()) {
      auto [d, r, c, dirs] = pq.front();
      d *= -1;
      pq.pop_front();
      if (r == goal.first && c == goal.second) return d;
      DEBUG(r, c, d);
      REP(j, 4) {
        if ((dirs & (1 << j)) == 0) continue;
        for (int k = 1; k <= K; ++k) {
          int nr = r + dx[j] * k;
          int nc = c + dy[j] * k;
          if (nr < 0 || nr >= H) break;
          if (nc < 0 || nc >= W) break;
          if (grid[nr][nc] == '@') break;
          if (distm[nr][nc] <= d + 1) continue;
          distm[nr][nc] = d + 1;
          int ndirs = -1 & ~(1 << rev[j]);
          if (k < K) {
            ndirs &= ~(1 << j);
          }
          pq.emplace_back(-(d + 1), nr, nc, ndirs);
        }
      }
    }
    return -1LL;
  };
  cout << solve() << endl;
}
