#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)

// Partially Persistent UnionFind.
struct UnionFindWithTime {
  int n;
  mutable std::vector<int> parent_;  // positive: parent, negative: size
  int num_roots_;
  int clock_;
  std::vector<int> parented_time_;
  std::vector<std::vector<std::pair<int, int>>> size_history_;

  explicit UnionFindWithTime(int sz)
      : n(sz),
        parent_(sz, -1),
        num_roots_(sz),
        clock_(0),
        parented_time_(sz, -1),
        size_history_(n, {{0, 1}}) {}

  // Returns current clock_.
  int unite(int x, int y) {
    ++clock_;
    x = find(x, clock_), y = find(y, clock_);
    if (x == y) return clock_;
    if (parent_[x] > parent_[y]) std::swap(x, y);  // Ensure size(x) > size(y).
    parent_[x] += parent_[y];
    parent_[y] = x;
    parented_time_[y] = clock_;
    size_history_[x].emplace_back(clock_, -parent_[x]);
    --num_roots_;
    return clock_;
  }

  int find(int v, int time) const {
    if (parent_[v] < 0) return v;
    if (time < parented_time_[v]) return v;
    return find(parent_[v], time);
  }
  int find(int v) const { return find(v, clock_); }

  int size(int v, int time) const {
    int r = find(v, time);
    const auto &h = size_history_[r];
    auto it = std::lower_bound(h.begin(), h.end(), std::pair(time + 1, -1));
    return (--it)->second;
  }
  int size(int v) const { return -parent_[find(v)]; }

  bool same(int x, int y, int time) const {
    return find(x, time) == find(y, time);
  }
  bool same(int x, int y) const { return find(x) == find(y); }

  std::optional<int> united_time(int x, int y) {
    if (not same(x, y)) {
      return std::nullopt;
    }
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

using namespace std;

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);

  int n, m;
  cin >> n >> m;

  UnionFindWithTime uf(n);
  REP(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    uf.unite(a, b);
  }

  int q;
  cin >> q;
  REP(i, q) {
    int x, y;
    cin >> x >> y;
    --x, --y;
    cout << uf.united_time(x, y).value_or(-1) << "\n";
  }
}
