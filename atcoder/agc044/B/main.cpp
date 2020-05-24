#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

void pd(const vector<vector<int>>& dist, const vector<vector<bool>>& left) {
  REP(i, dist.size()) {
    REP(j, dist.size()) {
      if (left[i][j])
        cerr << ' ';
      else
        cerr << dist[i][j];
    }
    cerr << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  vector<tuple<int, int>> steps = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

  int N;
  cin >> N;
  vector<int> P(N * N);
  for (auto& x : P) {
    cin >> x;
    x--;
  }

  vector<vector<int>> dist(N, vector<int>(N));
  vector<vector<bool>> left(N, vector<bool>(N));
  REP(i, N) {
    REP(j, N) {
      int d = N;
      d = min(d, i);
      d = min(d, j);
      d = min(d, N - 1 - i);
      d = min(d, N - 1 - j);
      dist[i][j] = d;
    }
  }

  auto upd = [&](int r0, int c0, i64 d0) -> void {
    deque<tuple<int, int, i64>> q;
    q.emplace_back(r0, c0, d0);
    // set<tuple<int, int>> vis;
    while (q.size()) {
      auto [r, c, d] = q.front();
      q.pop_front();
      //   auto rc = make_tuple(r, c);
      //   if (vis.count(rc)) {
      //     continue;
      //   }
      //   vis.insert(rc);
      for (auto [drow, dcol] : steps) {
        int row2 = r + drow;
        int col2 = c + dcol;
        if (row2 < 0 || row2 >= N || col2 < 0 || col2 >= N) continue;
        i64 s = d + int(!left[r][c]);
        if (dist[row2][col2] > s) {
          dist[row2][col2] = s;
          q.emplace_back(row2, col2, s);
        }
      }
    }
  };

  i64 total_cost = 0;
  //  int i = 0;
  for (const auto& p : P) {
    int col = p % N;
    int row = p / N;
    left[row][col] = true;
    total_cost += dist[row][col];
    upd(row, col, dist[row][col]);

    // cerr << "--- step " << (i++) << ": (" << row << "-" << col << ") "
    //      << "--------------------------------------------------------------"
    //      << endl;
    // pd(dist, left);
  }
  cout << total_cost << endl;
}