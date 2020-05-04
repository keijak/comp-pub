#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

void solve(int64 N, long long M, std::vector<vector<int>> adj) {
  unordered_map<int64, int64> sub;
  auto dfs = [&](auto rec, int node, int parent) -> int64 {
    if (parent >= 0) {
      auto it = sub.find(static_cast<int64>(node) * N + parent);
      if (it != sub.end()) {
        return it->second;
      }
    }
    int64 r = 1;
    for (int child : adj[node]) {
      if (child == parent) continue;
      r = r * (1 + rec(rec, child, node)) % M;
    }
    if (parent >= 0) {
      sub[static_cast<int64>(node) * N + parent] = r;
    }
    return r;
  };
  for (int i = 0; i < N; ++i) {
    cout << dfs(dfs, i, -1) << "\n";
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  long long N, M;
  cin >> N >> M;
  vector<vector<int>> adj(N);
  for (int i = 0; i < N - 1; i++) {
    int x, y;
    cin >> x >> y;
    x--;
    y--;
    adj[x].push_back(y);
    adj[y].push_back(x);
  }
  solve(N, M, std::move(adj));
}
