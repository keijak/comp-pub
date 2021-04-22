#include <bits/stdc++.h>

#include <atcoder/scc>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
using i64 = long long;

using namespace std;

i64 solve() {
  int n, m;
  cin >> n >> m;
  atcoder::scc_graph g(n);
  REP(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g.add_edge(a, b);
  }
  i64 ans = 0;
  for (const auto &co : g.scc()) {
    const i64 k = co.size();
    ans += k * (k - 1) / 2;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
