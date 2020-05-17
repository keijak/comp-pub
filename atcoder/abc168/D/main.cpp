#pragma GCC optimize("O3")
#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  vector<vector<int>> adj(N);
  REP(i, N) { adj[i].reserve(M); }
  REP(i, M) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    adj[a].push_back(b);
    adj[b].push_back(a);
  }
  deque<tuple<int, int, int>> q;
  q.emplace_back(0, 0, -1);
  int INF = N + 10;
  vector<int> dist(N, INF);
  vector<int> prev(N, -1);
  int cnt = 0;
  while (!q.empty()) {
    auto [l, node, pnode] = q.front();
    q.pop_front();
    if (dist[node] <= l) continue;
    dist[node] = l;
    prev[node] = pnode;
    ++l;
    for (auto&& nxt : adj[node]) {
      if (dist[nxt] > l) {
        q.emplace_back(l, nxt, node);
      }
    }
  }
  cout << "Yes\n";
  for (int i = 1; i < N; ++i) {
    cout << (prev[i] + 1) << "\n";
  }
}
