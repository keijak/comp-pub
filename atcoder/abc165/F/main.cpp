#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

void solve(long long N, std::vector<long long> a, std::vector<long long> u,
           std::vector<long long> v) {
  vector<int64> dp(N, 0);
  dp[0] = 1;
  deque<tuple<int, int>> q;
  q.emplace_back(0, -1);
  while (!q.empty()) {
    auto [node, parent] = q.front();
    q.pop_front();
  }
}

int main() {
  int N;
  cin >> N;
  std::vector<long long> a(N);
  for (int i = 0; i < N; i++) {
    scanf("%lld", &a[i]);
  }
  std::vector<vector<int>> adj(N);
  for (int i = 0; i < N - 1; i++) {
    int u, v;
    cin >> u >> v;
    u--;
    v--;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  solve(N, std::move(a), std::move(u), std::move(v));
}
