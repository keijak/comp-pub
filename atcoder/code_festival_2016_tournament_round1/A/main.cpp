#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

using namespace std;

// Partially Persistent UnionFind.
struct UnionFindWithTime {
  int n;
  mutable std::vector<int> parent_;  // positive: parent, negative: size
  std::vector<int> rank_;
  int clock_;
  std::vector<int> parented_time_;

  explicit UnionFindWithTime(int sz)
      : n(sz),
        parent_(sz, -1),
        rank_(sz, 1),
        clock_(0),
        parented_time_(sz, -1) {}

  // Returns current clock_.
  int unite(int x, int y) {
    ++clock_;
    x = find(x, clock_), y = find(y, clock_);
    if (x == y) return clock_;
    if (rank_[x] < rank_[y] or
        (rank_[x] == rank_[y] and -parent_[x] < -parent_[y])) {
      std::swap(x, y);
    }
    parent_[x] += parent_[y];
    parent_[y] = x;
    rank_[x] = std::max(rank_[x], rank_[y] + 1);
    parented_time_[y] = clock_;
    return clock_;
  }

  int find(int v, int time) const {
    if (parent_[v] < 0) return v;
    if (time < parented_time_[v]) return v;
    return find(parent_[v], time);
  }
  int find(int v) const { return find(v, clock_); }

  bool same(int x, int y, int time) const {
    return find(x, time) == find(y, time);
  }
  bool same(int x, int y) const { return find(x) == find(y); }

  int united_time(int x, int y) {
    int fv = 0, tv = clock_;
    while (tv - fv > 1) {
      int mid = (tv + fv) / 2;
      if (same(x, y, mid)) {
        tv = mid;
      } else {
        fv = mid;
      }
    }
    return tv;
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vector<array<int, 3>> edges(m);
  REP(i, m) {
    int a, b, c;
    cin >> a >> b >> c;
    --a, --b;
    edges[i] = {c, a, b};
  }
  sort(ALL(edges));

  i64 total_cost = 0;
  UnionFindWithTime uf(n);
  vector<int> cmap(5000);
  for (auto [c, a, b] : edges) {
    if (uf.same(a, b)) continue;
    int t = uf.unite(a, b);
    total_cost += c;
    cmap[t] = c;
  }

  int q;
  cin >> q;
  REP(i, q) {
    int s, t;
    cin >> s >> t;
    --s, --t;
    auto j = uf.united_time(s, t);
    i64 ans = total_cost - cmap[j];
    cout << ans << "\n";
  }
}
