#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

using namespace std;

// tour: preorder node ids
// The interval [pre_index[v], post_index[v]) represents a subtree whose
// root is v.
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G adj;

  // Euler Tour on nodes.
  vector<int> tour;        // nodes
  vector<int> pre_index;   // index in the tour on entering the subtree of v
  vector<int> post_index;  // index in the tour on exiting the subtree of v
  vector<int> level;

  explicit EulerTour(G g, int root = 0)
      : n((int)g.size()),
        adj(move(g)),
        pre_index(n, -1),
        post_index(n, -1),
        level() {
    tour.reserve(n);
    dfs(root, 0);
  }

 private:
  void dfs(int v, int lv) {
    pre_index[v] = int(tour.size());
    tour.push_back(v);
    level.push_back(lv);
    for (auto u : adj[v]) {
      dfs(u, lv + 1);
      tour.push_back(v);
      level.push_back(lv);
    }
    post_index[v] = int(tour.size());
  }
};

template <class SemiLattice>
struct SparseTable {
  using T = typename SemiLattice::T;

  explicit SparseTable(const std::vector<T> &vec) { init(vec); }

  // Queries by [l,r) range (0-indexed, half-open interval).
  T fold(int l, int r) const {
    l = max(l, 0);
    r = min<int>(r, n_);
    if (l >= r) {
      return SemiLattice::id();
    }
    const T &lval = data_[height_[r - l]][l];
    const T &rval = data_[height_[r - l]][r - (1 << height_[r - l])];
    return SemiLattice::op(lval, rval);
  }

  // Returns i-th value (0-indexed).
  T operator[](int i) const { return data_[height_[1]][i]; }

 private:
  void init(const std::vector<T> &vec) {
    int n = vec.size(), h = 0;
    n_ = n;
    while ((1 << h) <= n) ++h;
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

  int n_;  // number of elements.
  std::vector<std::vector<T>> data_;
  std::vector<int> height_;
};

const int BIG = 1000000;

struct Min {
  using T = int;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return BIG; }
};

struct Max {
  using T = int;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return -BIG; }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n, q;
  cin >> n >> q;
  vector<int> parent(n, -1);
  vector<vector<int>> children(n);
  REP(i, n - 1) {
    int p;
    cin >> p;
    --p;
    parent[i + 1] = p;
    children[p].push_back(i + 1);
  }
  EulerTour et(children, 0);

  SparseTable<Min> minst(et.pre_index);
  SparseTable<Max> maxst(et.pre_index);
  SparseTable<Min> levelst(et.level);

  REP(iq, q) {
    int l, r;
    cin >> l >> r;
    --l;
    int imin = minst.fold(l, r);
    int imax = maxst.fold(l, r);
    int manager_level = -1;
    int excluded = -1;
    {  // exclude imin
      int ei = et.tour[imin];
      int mi = BIG;
      chmin(mi, minst.fold(l, ei));
      chmin(mi, minst.fold(ei + 1, r));
      manager_level = levelst.fold(mi, imax + 1);
      excluded = ei;
    }
    {  // exclude imax
      int ei = et.tour[imax];
      int mi = -BIG;
      chmax(mi, maxst.fold(l, ei));
      chmax(mi, maxst.fold(ei + 1, r));
      if (chmax(manager_level, levelst.fold(imin, mi + 1))) {
        excluded = ei;
      }
    }
    cout << excluded + 1 << " " << manager_level << "\n";
  }
}
