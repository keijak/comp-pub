#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)

using namespace std;

struct Diameter {
  int distance;
  array<int, 2> nodes;
};

// Tree diameter.
// Returns the maximum diameter and two end point nodes with the diameter.
Diameter tree_diameter(const std::vector<std::vector<int>> &g) {
  auto farthest = [&](auto &rec, int v, int p, int d) -> std::pair<int, int> {
    int res_d = d, res_v = v;
    for (auto u : g[v]) {
      if (u == p) continue;
      const auto &[child_d, child_v] = rec(rec, u, v, d + 1);
      if (child_d > res_d) {
        res_d = child_d;
        res_v = child_v;
      }
    }
    return {res_d, res_v};
  };

  const auto &[_, u] = farthest(farthest, 0, -1, 0);
  const auto &[distance, v] = farthest(farthest, u, -1, 0);
  return Diameter{distance, {u, v}};
}

int solve() {
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
  auto res = tree_diameter(g);
  return res.distance + 1;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
