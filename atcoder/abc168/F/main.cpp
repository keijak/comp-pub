#include <algorithm>
#include <deque>
#include <iostream>
#include <optional>
#include <vector>

#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using i64 = long long;
using u64 = unsigned long long;

int N, M;
i64 INF = 1LL << 60;

optional<u64> bfs(const vector<i64>& xv, const vector<i64>& yv,
                  vector<vector<char>>& grid) {
  vector<tuple<int, int>> steps{{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  deque<tuple<int, int>> que;
  int c0 = lower_bound(xv.begin(), xv.end(), 0) - xv.begin();
  int r0 = lower_bound(yv.begin(), yv.end(), 0) - yv.begin();
  que.emplace_back(2 * r0 + 1, 2 * c0 + 1);

  while (que.size()) {
    const auto& [r, c] = que.front();
    que.pop_front();
    for (const auto& [dr, dc] : steps) {
      int rn = r + dr, cn = c + dc;
      if (rn < 0 || rn >= int(grid.size()) || cn < 0 ||
          cn >= int(grid[0].size())) {
        return nullopt;
      }
      if (grid[rn][cn] != 0) {
        continue;
      }
      grid[rn][cn] = 1;
      que.emplace_back(rn, cn);
    }
  }

  u64 area = 0;
  REP(r, yv.size()) {
    REP(c, xv.size()) {
      char s = grid[2 * r + 1][2 * c + 1];
      if (s == 1) {
        area += (xv[c + 1] - xv[c]) * (yv[r + 1] - yv[r]);
      }
    }
  }
  return area;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> M;
  vector<tuple<i64, i64, i64>> verticals(N);
  vector<tuple<i64, i64, i64>> horizontals(M);
  vector<i64> xv, yv;
  xv.reserve(N + 2 * M + 10);
  yv.reserve(2 * N + M + 10);
  xv.push_back(0);
  yv.push_back(0);
  yv.push_back(INF);
  yv.push_back(-INF);
  REP(i, N) {  // vertical lines
    i64 a, b, c;
    cin >> a >> b >> c;
    verticals[i] = make_tuple(a, b, c);
    yv.push_back(a);
    yv.push_back(b);
    xv.push_back(c);
  }
  REP(i, M) {  // horizontal lines
    i64 d, e, f;
    cin >> d >> e >> f;
    horizontals[i] = make_tuple(d, e, f);
    yv.push_back(d);
    xv.push_back(e);
    xv.push_back(f);
  }
  sort(xv.begin(), xv.end());
  xv.erase(unique(xv.begin(), xv.end()), xv.end());
  sort(yv.begin(), yv.end());
  yv.erase(unique(yv.begin(), yv.end()), yv.end());
  vector<vector<char>> grid(yv.size() * 2, vector<char>(xv.size() * 2, 0));
  REP(i, N) {
    auto [a, b, c] = verticals[i];
    int r1 = lower_bound(yv.begin(), yv.end(), a) - yv.begin();
    int r2 = lower_bound(yv.begin(), yv.end(), b) - yv.begin();
    int ci = lower_bound(xv.begin(), xv.end(), c) - xv.begin();
    for (int rd = 2 * r1; rd < 2 * r2; ++rd) {
      grid[rd][2 * ci] = 2;
    }
  }
  REP(i, M) {
    auto [d, e, f] = horizontals[i];
    int ri = lower_bound(yv.begin(), yv.end(), d) - yv.begin();
    int c1 = lower_bound(xv.begin(), xv.end(), e) - xv.begin();
    int c2 = lower_bound(xv.begin(), xv.end(), f) - xv.begin();
    for (int cd = 2 * c1; cd < 2 * c2; ++cd) {
      grid[2 * ri][cd] = 2;
    }
  }

  auto result = bfs(xv, yv, grid);
  if (!result) {
    cout << "INF" << endl;
  } else {
    cout << *result << endl;
  }
}
