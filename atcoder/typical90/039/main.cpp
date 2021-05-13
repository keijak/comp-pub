#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

using namespace std;

i64 solve() {
  int n;
  cin >> n;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  i64 ans = 0;
  auto dfs_size = [&](auto &rec, int v, int p) -> i64 {
    i64 sub_size = 1;
    for (auto u : g[v]) {
      if (u == p) continue;
      const i64 a_size = rec(rec, u, v);
      const i64 b_size = n - a_size;
      ans += a_size * b_size;
      sub_size += a_size;
    }
    return sub_size;
  };
  dfs_size(dfs_size, 0, -1);
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
