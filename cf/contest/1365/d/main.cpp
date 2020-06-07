#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
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

bool solve(int N, int M) {
  vector<string> grid(N);
  vector<pair<int, int>> bads;
  int good_cnt = 0;
  REP(i, N) {
    getline(cin, grid[i]);
    assert(grid[i].size() == size_t(M));
    REP(j, M) {
      if (grid[i][j] == 'G') good_cnt++;
      if (grid[i][j] == 'B') {
        bads.emplace_back(i, j);
      }
    }
  }
  if (good_cnt == 0) {
    return true;  // Block the escape.
  }

  auto block_bad = [&](int r, int c) -> bool {
    if (r < 0 || r >= N) return true;
    if (c < 0 || c >= M) return true;
    char cell = grid[r][c];
    switch (cell) {
      case 'G':
        return false;
      case '.':
        grid[r][c] = '#';
        break;
    }
    return true;
  };

  for (auto [r, c] : bads) {
    // Fail if a good person is next to a bad person.
    if (!block_bad(r - 1, c)) return false;
    if (!block_bad(r + 1, c)) return false;
    if (!block_bad(r, c - 1)) return false;
    if (!block_bad(r, c + 1)) return false;
  }
  // Escape is blocked.
  if (grid[N - 1][M - 1] == '#') return false;
  assert(grid[N - 1][M - 1] == '.');

  vector<int> dx = {0, 0, 1, -1};
  vector<int> dy = {1, -1, 0, 0};
  // DFS from escape to collect all good person.
  auto dfs = [&](auto rec, int r, int c) -> bool {
    grid[r][c] = '#';
    REP(i, 4) {
      int nr = r + dy[i];
      int nc = c + dx[i];
      if (nr < 0 || nr >= N) continue;
      if (nc < 0 || nc >= M) continue;
      char cell = grid[nr][nc];
      if (cell == '#') continue;
      if (cell == 'G') {
        if (--good_cnt == 0) return true;
      } else {
        assert(cell == '.');
      }
      if (rec(rec, nr, nc)) return true;
    }
    return false;
  };
  return dfs(dfs, N - 1, M - 1);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int n, m;
    cin >> n >> m;
    cin.ignore();
    cout << (solve(n, m) ? "Yes\n" : "No\n");
  }
}
