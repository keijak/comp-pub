#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(std::move(v)) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }
};

template<typename T>
struct MergeSegmentTree {
  int n_;
  std::vector<std::vector<T>> data_;  // Rows sorted by T.

  explicit MergeSegmentTree(std::vector<std::vector<T>> data)
      : n_(data.size()), data_(2 * n_) {
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

using namespace std;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  const int n = in, Q = in;
  vector<pair<int, int>> xy(n);
  vector<int> xs(n);
  REP(i, n) {
    int x = in, y = in;
    xy[i] = {x, y};
    xs[i] = x;
  }
  Compressed<int> xc(xs);
  vector<vector<int>> ys(xc.size());
  for (auto[x, y]: xy) {
    int xi = xc.index(x);
    ys[xi].push_back(y);
  }
  MergeSegmentTree<int> st(ys);
  REP(i, Q) {
    int l1 = in, l2 = in, r1 = in, r2 = in;
    int j1 = xc.index(l1), j2 = xc.index(l2 + 1);
    int res = st.count(j1, j2, r1, r2 + 1);
    cout << res << '\n';
  }
}
