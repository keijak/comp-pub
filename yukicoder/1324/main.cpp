#include <bits/stdc++.h>

#include <atcoder/mincostflow>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), N_##i = (b); i < N_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}

using namespace std;

i64 solve() {
  int n, k;
  cin >> n >> k;
  vector<int> a(n), b(n);
  cin >> a >> b;
  vector p(n, vector(n, 0));
  i64 p_norm = 0;
  REP(i, n) REP(j, n) {
    cin >> p[i][j];
    p_norm += p[i][j] * p[i][j];
  }

  atcoder::mcf_graph<int, i64> g(n * 2 + 2);
  const int v_source = n * 2;
  const int v_sink = n * 2 + 1;

  REP(i, n) { g.add_edge(v_source, i, a[i], 0); }
  REP(i, n) { g.add_edge(n + i, v_sink, b[i], 0); }
  const i64 BASE_COST = 600;
  REP(i, n) REP(j, n) {
    for (i64 x = 1; x <= min(a[i], b[j]); ++x) {
      // (x - p)^2 - (x - 1 - p)^2
      // = (x^2 - 2px + p^2) - (x^2 + 1 + p^2 - 2x + 2p - 2px)
      // = 2x - 2p - 1
      i64 cost = 2 * x - 2 * p[i][j] - 1 + BASE_COST;
      g.add_edge(i, n + j, 1, cost);
    }
  }
  auto [maxflow, mincost] = g.flow(v_source, v_sink, k);
  assert(maxflow == k);
  return mincost - k * BASE_COST + p_norm;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
