#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
using namespace std;

template <typename T>
using V = vector<T>;

// Lowest Common Ancestor by EulerTour + RMQ (SparseTable).
// - query: O(1)
// - build: O(N log N)
// - space: O(N log N)
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G adj;
  vector<int> depth;
  vector<int> index;
  // Euler Tour on nodes.
  vector<pair<int, int>> tour;  // (depth, node id)

  explicit EulerTour(G g, int root = 0)
      : n(g.size()), adj(move(g)), depth(n, 0), index(n, -1), tour() {
    tour.reserve(n * 2);
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    index[v] = int(tour.size());
    depth[v] = (p < 0) ? 0 : (depth[p] + 1);
    tour.emplace_back(depth[v], v);
    for (auto u : adj[v]) {
      if (u == p) continue;
      dfs(u, v);
      tour.emplace_back(depth[v], v);
    }
  }
};

template <class SemiLattice>
struct SparseTable {
  using T = typename SemiLattice::T;

  explicit SparseTable(const std::vector<T> &vec) { init(vec); }

  T fold(int l, int r) const {
    const T &lval = data_[height_[r - l]][l];
    const T &rval = data_[height_[r - l]][r - (1 << height_[r - l])];
    return SemiLattice::op(lval, rval);
  }

 private:
  void init(const std::vector<T> &vec) {
    int n = vec.size(), h = 0;
    while ((1 << h) < n) ++h;
    data_.assign(h, std::vector<T>(1 << h, SemiLattice::id()));
    height_.assign(n + 1, 0);
    for (int i = 2; i <= n; i++) {
      height_[i] = height_[i >> 1] + 1;
    }
    for (int i = 0; i < n; ++i) {
      data_[0][i] = vec[i];
    }
    for (int i = 1; i < h; ++i)
      for (int j = 0; j < n; ++j)
        data_[i][j] = SemiLattice::op(
            data_[i - 1][j], data_[i - 1][std::min(j + (1 << (i - 1)), n - 1)]);
  }

  std::vector<std::vector<T>> data_;
  std::vector<int> height_;
};

struct Min {
  using T = pair<int, int>;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return {std::numeric_limits<int>::max(), 0}; }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  V<V<int>> g(N);
  REP(i, N - 1) {
    int p;
    cin >> p;
    g[p].push_back(i + 1);
  }

  EulerTour et(move(g));
  SparseTable<Min> st(et.tour);

  REP(i, Q) {
    int u, v;
    cin >> u >> v;

    int ui = et.index[u], vi = et.index[v];
    auto [lca_depth, lca_id] = st.fold(min(ui, vi), max(ui, vi) + 1);
    cout << lca_id << '\n';
  }
}
