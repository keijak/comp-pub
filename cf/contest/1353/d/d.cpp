/* Time: 2020-05-14 17:25:41 */

#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

vector<int64> solve(int64 n) {
  vector<int64> ans(n, 0);
  priority_queue<tuple<int, int>> pq;
  pq.emplace(n, 0);
  for (int i = 1; i <= n; ++i) {
    if (pq.empty()) break;
    auto [len, start] = pq.top();
    pq.pop();
    start = -start;
    // cout << ">>  len:" << len << " at " << start << endl;
    int fill;
    if (len % 2 == 1) {
      fill = start + len / 2;
    } else {
      fill = start + (len - 1) / 2;
    }
    // cout << ">>" << fill << "=>" << i << endl;
    ans[fill] = i;
    if (fill > start) {
      pq.emplace(fill - start, -start);
    }
    if (start + len - fill - 1 > 0) {
      pq.emplace(start + len - fill - 1, -(fill + 1));
    }
  }

  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int T;
  cin >> T;
  while (T--) {
    int64 n;
    cin >> n;
    auto v = solve(n);
    REP(i, v.size()) {
      if (i > 0) cout << ' ';
      cout << v[i];
    }
    cout << endl;
  }
}
