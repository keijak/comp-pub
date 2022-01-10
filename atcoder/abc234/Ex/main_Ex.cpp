#include <bits/stdc++.h>

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
using Int = long long;

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);

  int n, K;
  cin >> n >> K;
  const Int K2 = Int(K) * K;

  vector<pair<Int, Int>> points(n);
  vector<Int> xs;
  for (auto&[x, y]: points) {
    cin >> x >> y;
    xs.push_back(x);
  }
  Compressed<Int> xc(xs);
  vector<vector<pair<Int, int>>> data(xc.size());
  for (int i = 0; i < n; ++i) {
    auto[x, y] = points[i];
    int xi = xc.index(x);
    data[xi].push_back({y, i});
  }

  MergeSegmentTree<pair<Int, int>> tree(data);
  vector<pair<int, int>> ans;
  for (int i = 1; i < n; ++i) {
    auto[x, y] = points[i];
    vector<pair<Int, int>> res = tree.collect(xc.index(x - K), xc.index(x + K + 1), {y - K, -1}, {y + K + 1, -1});
    for (auto[_, j]: res) {
      if (j < i) {
        Int dx = x - points[j].first, dy = y - points[j].second;
        if (dx * dx + dy * dy <= K2) {
          ans.emplace_back(j, i);
        }
      }
    }
  }
  sort(ans.begin(), ans.end());
  cout << ans.size() << '\n';
  for (auto[p, q]: ans) {
    cout << (p + 1) << ' ' << (q + 1) << '\n';
  }
}
