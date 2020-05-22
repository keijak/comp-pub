#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
const i64 INF = 1LL << 60;

int main() {
  int N;
  cin >> N;
  vector<i64> A(N);
  for (auto& x : A) {
    cin >> x;
  }
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
  auto solve = [&](auto rec, int node, int pnode = -1) -> void {
    assert(answer[node] == 0);
    auto it = lower_bound(lis.begin(), lis.end(), A[node]);
    assert(it != lis.end());
    const int j = distance(lis.begin(), it);
    answer[node] = 1 + j;
    if (pnode != -1) {
      answer[node] = max(answer[node], answer[pnode]);
    }
    i64 backup = lis[j];
    lis[j] = A[node];
    for (int cnode : adj[node]) {
      if (cnode != pnode) {
        rec(rec, cnode, node);
      }
    }
    lis[j] = backup;
  };
  solve(solve, 0);

  for (int x : answer) {
    cout << x << "\n";
  }
}
