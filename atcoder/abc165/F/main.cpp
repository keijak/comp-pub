#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
const i64 INF = 100000000000LL;

int main() {
  int N;
  cin >> N;
  vector<i64> a(N);
  REP(i, N) { cin >> a[i]; }
  vector<vector<int>> adj(N);
  REP(i, N - 1) {
    int u, v;
    cin >> u >> v;
    u--;
    v--;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  vector<int> answer(N);
  vector<i64> lis(N + 1, INF);
  auto solve = [&](auto rec, int node, int pnode) -> void {
    assert(answer[node] == 0);
    auto it = lower_bound(lis.begin(), lis.end(), a[node]);
    assert(it != lis.end());
    const int j = distance(lis.begin(), it);
    answer[node] = 1 + j;
    i64 bak = lis[j];
    lis[j] = a[node];
    for (int c : adj[node]) {
      if (c != pnode) {
        rec(rec, c, node);
      }
    }
    lis[j] = bak;
  };
  solve(solve, 0, -1);

  for (int x : answer) {
    assert(x > 0);
    assert(x <= N);
    cout << x << endl;
  }
}
