#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)

struct Point {
  using T = int;
  T x, y;
  int index;
};

// Merge Segment Tree
//
// 二次元座標上の点集合を扱う
// - 矩形領域内にある点の数を返す: O(N(logN)^2)
// - 矩形領域内にある点を列挙する: O(N(logN)^2 + |output|)
// 座標の第一要素(X)はセグメントツリーのインデックスとする。必要に応じて座標圧縮すること。
// 座標の第二要素(Y)は座標圧縮する必要はない。
// 点に座標以外の情報を持たせる場合は T = pair(Y, index) として collect() してから
// 元データに index でアクセスするとよい。

template<typename T>
struct MergeSegmentTree {
  int n_;
  std::vector<std::vector<T>> data_;

  explicit MergeSegmentTree(std::vector<std::vector<T>> data) : n_(data.size()), data_(2 * n_) {
    for (int i = n_, n2 = n_ * 2; i < n2; ++i) {
      data_[i] = std::move(data[i - n_]);
      std::sort(data_[i].begin(), data_[i].end());
    }
    for (int i = n_ - 1; i >= 1; --i)
      std::merge(data_[i << 1 | 0].begin(), data_[i << 1 | 0].end(),
                 data_[i << 1 | 1].begin(), data_[i << 1 | 1].end(),
                 std::back_inserter(data_[i]));
  }

  // Returns the nubmer of points in the range [x_lo, x_hi) x [y_lo, y_hi).
  // O(N(logN)^2)
  int count(int x_lo, int x_hi, T y_lo, T y_hi) const {
    int res = 0;
    for (x_lo += n_, x_hi += n_; x_lo < x_hi; x_lo >>= 1, x_hi >>= 1) {
      if (x_lo & 1) res += count_(x_lo++, y_lo, y_hi);
      if (x_hi & 1) res += count_(--x_hi, y_lo, y_hi);
    }
    return res;
  }

  // Returns all points in the range [x_lo, x_hi) x [y_lo, y_hi).
  // O(N(logN)^2 + |output|)
  std::vector<T> collect(int x_lo, int x_hi, T y_lo, T y_hi) const {
    std::vector<T> res;
    for (x_lo += n_, x_hi += n_; x_lo < x_hi; x_lo >>= 1, x_hi >>= 1) {
      if (x_lo & 1) collect_(x_lo++, y_lo, y_hi, res);
      if (x_hi & 1) collect_(--x_hi, y_lo, y_hi, res);
    }
    return res;
  }

 private:
  int count_(int i, T y_lo, T y_hi) const {
    auto lo_it = std::lower_bound(data_[i].begin(), data_[i].end(), y_lo);
    auto hi_it = std::lower_bound(data_[i].begin(), data_[i].end(), y_hi);
    return int(hi_it - lo_it);
  }
  void collect_(int i, T y_lo, T y_hi, std::vector<T> &out) const {
    auto lo_it = std::lower_bound(data_[i].begin(), data_[i].end(), y_lo);
    auto hi_it = std::lower_bound(data_[i].begin(), data_[i].end(), y_hi);
    std::copy(lo_it, hi_it, std::back_inserter(out));
  }
};

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(std::move(v)) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }
  int size() const { return values.size(); }
  int index(const T &x) const { return std::lower_bound(values.begin(), values.end(), x) - values.begin(); }
};

using namespace std;

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);

  int n;
  cin >> n;
  vector<Point> points(n);
  vector<int> xs(n);
  REP(i, n) {
    int x, y;
    cin >> x >> y;
    points[i] = {x, y, i};
    xs[i] = x;
  }
  Compressed<int> xc(xs);
  vector<vector<pair<int, int>>> data(xc.size());
  for (const Point &pt: points) {
    int xi = xc.index(pt.x);
    data[xi].emplace_back(pt.y, pt.index);
  }
  MergeSegmentTree<std::pair<int, int>> st(std::move(data));
  int q;
  cin >> q;
  REP(i, q) {
    int sx, tx, sy, ty;
    cin >> sx >> tx >> sy >> ty;
    int sxi = xc.index(sx);
    int txi = xc.index(tx + 1);
    vector<pair<int, int>> res = st.collect(sxi, txi, {sy, 0}, {ty + 1, 0});
    sort(ALL(res), [](const auto &p1, const auto &p2) { return p1.second < p2.second; });
    for (auto[y, j]: res) cout << j << '\n';
    cout << '\n';
  }
}
