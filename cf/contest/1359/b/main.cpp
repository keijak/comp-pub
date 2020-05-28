#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

i64 solve(vector<string>& grid, int x, int y) {
  i64 cost = 0;
  for (auto& row : grid) {
    int j = 0;
    int m = row.size();
    while (j < m) {
      if (row[j] == '*') {
        ++j;
      } else if (j == m - 1 || row[j + 1] == '*' || 2 * x <= y) {
        cost += x;
        ++j;
      } else {
        cost += y;
        j += 2;
      }
    }
  }

  return cost;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m, x, y;
    cin >> n >> m >> x >> y;
    cin.ignore();
    vector<string> grid(n);
    REP(i, n) {
      getline(cin, grid[i]);
      assert((int)grid[i].size() == m);
    }
    cout << solve(grid, x, y) << '\n';
  }
}