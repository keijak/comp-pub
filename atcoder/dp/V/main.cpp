#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using Int = long long;
Int N, M;
vector<vector<int>> adj;
vector<unordered_map<int, Int>> sub;
vector<vector<Int>> lprod, rprod;

void solve() {
  vector<Int> ans(N);
  auto dfs = [&](auto rec, int node, int parent) {
    if (parent >= 0) {
      auto it = sub[parent].find(node);
      if (it != sub[parent].end()) return it->second;
    }
    Int r = 1;
    vector<Int> cs(adj[node].size(), 1);
    REP(i, adj[node].size()) {
      int child = adj[node][i];
      if (child == parent) {
        cs[i] = 1;
        continue;
      }
      Int c = 1 + rec(rec, child, node);
      cs[i] = c;
      r = r * c % M;
    }
    lprod[node].resize(cs.size() + 1, 1);
    rprod[node].resize(cs.size() + 1, 1);
    REP(i, cs.size()) {
      lprod[node][i + 1] = lprod[node][i] * cs[i] % M;
      rprod[node][cs.size() - 1 - i] =
          rprod[node][cs.size() - i] * cs[cs.size() - 1 - i] % M;
    }
    if (parent >= 0) {
      sub[parent][node] = r;
    }
    return r;
  };
  ans[0] = dfs(dfs, 0, -1);

  auto dfs2 = [&](auto rec, int node, int parent, Int acc) -> void {
    REP(i, adj[node].size()) {
      int child = adj[node][i];
      if (child == parent) continue;
      ans[child] = sub[node][child];
      Int q = lprod[node][i] * rprod[node][i + 1] % M * (1 + acc) % M;
      ans[child] += sub[node][child] * q % M;
      ans[child] %= M;
      rec(rec, child, node, q);
    }
  };
  dfs2(dfs2, 0, -1, 0);
  for (int i = 0; i < N; ++i) {
    cout << ans[i] << "\n";
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cin >> N >> M;
  adj.resize(N);
  sub.resize(N);
  lprod.resize(N);
  rprod.resize(N);
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
