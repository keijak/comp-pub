#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using Int = long long;
Int N, M;
vector<vector<int>> adj;
vector<unordered_map<int, Int>> sub;

Int dfs(int node, int parent) {
  if (parent >= 0) {
    auto it = sub[parent].find(node);
    if (it != sub[parent].end()) return it->second;
  }
  Int r = 1;
  for (int child : adj[node]) {
    if (child == parent) continue;
    r = r * (1 + dfs(child, node)) % M;
  }
  if (parent >= 0) {
    sub[parent][node] = r;
  }
  return r;
};

void solve() {
  for (int i = 0; i < N; ++i) {
    cout << dfs(i, -1) << "\n";
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cin >> N >> M;
  adj.resize(N);
  sub.resize(N);
  for (int i = 0; i < N - 1; i++) {
    int x, y;
    cin >> x >> y;
    x--;
    y--;
    adj[x].push_back(y);
    adj[y].push_back(x);
  }
  solve();
}
