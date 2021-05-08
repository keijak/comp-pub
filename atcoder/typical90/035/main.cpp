#pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <limits>
#include <type_traits>
#include <vector>

struct Input {
  inline operator int() { return read_int<int>(); }
  inline operator long long() { return read_int<long long>(); }
  inline operator unsigned() { return read_int<unsigned>(); }

 private:
  template <typename T>
  static T read_int() {
    T ret = 0, sgn = 1;
    int ch = getchar_unlocked();
    while (isspace(ch)) {
      ch = getchar_unlocked();
    }
    if constexpr (!std::is_unsigned<T>::value) {
      if (ch == '-') {
        sgn = -1;
        ch = getchar_unlocked();
      }
    }
    for (; ('0' <= ch) & (ch <= '9'); ch = getchar_unlocked()) {
      ret = (ret * 10) + (ch - '0');
    }
    ungetc(ch, stdin);
    if constexpr (std::is_unsigned<T>::value) {
      return ret;
    } else {
      return ret * sgn;
    }
  }
} input;

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)

using namespace std;

template <class SemiLattice>
struct SparseTable {
  using T = typename SemiLattice::T;

  explicit SparseTable(const std::vector<T> &vec) { init(vec); }

  // Queries by [l,r) range (0-indexed, half-open interval).
  T fold(int l, int r) const {
    l = std::max(l, 0);
    r = std::min(r, n_);
    if (l >= r) {
      return SemiLattice::id();
    }
    const T &lval = data_[height_[r - l]][l];
    const T &rval = data_[height_[r - l]][r - (1 << height_[r - l])];
    return SemiLattice::op(lval, rval);
  }

  // Returns i-th value (0-indexed).
  T operator[](int i) const {
    assert(0 <= i and i < n_);
    return data_[height_[1]][i];
  }

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

struct Min {
  using T = int;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

// tour: preorder node ids
// The interval [begin[v], end[v]) represents a subtree whose
// root is v.
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G g;

  // Euler Tour on nodes.
  vector<int> tour;   // depth
  vector<int> begin;  // index in the tour on entering the subtree of v
  vector<int> end;    // index in the tour on exiting the subtree of v
  vector<int> depth;

  explicit EulerTour(G g, int root = 0)
      : n((int)g.size()), g(move(g)), begin(n, -1), end(n, -1), depth(n, -1) {
    tour.reserve(n * 2);
    depth[root] = 0;
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    begin[v] = int(tour.size());
    if (p >= 0) depth[v] = depth[p] + 1;
    tour.push_back(depth[v]);
    for (auto u : g[v]) {
      if (u == p) continue;
      dfs(u, v);
      tour.push_back(depth[v]);
    }
    end[v] = int(tour.size());
  }
};

int main() {
  const int n = input;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    const int a = int(input) - 1;
    const int b = int(input) - 1;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  EulerTour et(g);
  SparseTable<Min> st(et.tour);
  vector<int> ord;
  ord.reserve(200005);
  const int q = input;
  REP(_, q) {
    const int k = input;
    ord.resize(k);
    REP(i, k) {
      const int v = int(input) - 1;
      ord[i] = et.begin[v];
    }
    sort(ALL(ord));
    const auto lca_depth = st.fold(ord.front(), ord.back() + 1);
    int res = et.tour[ord[0]] - lca_depth;
    for (int i = 1; i < k; ++i) {
      const auto ld = st.fold(ord[i - 1], ord[i] + 1);
      res += et.tour[ord[i]] - ld;
    }
    printf("%d\n", res);
  }
}
