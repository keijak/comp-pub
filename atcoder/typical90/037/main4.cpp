#include <bits/stdc++.h>
using Int = long long;

template <typename Monoid>
struct SegTree {
  using T = typename Monoid::T;

 private:
  int n_;                // number of valid leaves.
  int offset_;           // where leaves start
  std::vector<T> data_;  // data size: 2*offset_

 public:
  inline int n() const { return n_; }
  inline int offset() const { return offset_; }

  explicit SegTree(int n) : n_(n) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
  }

  explicit SegTree(const std::vector<T> &leaves) : n_(leaves.size()) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
    for (int i = 0; i < n_; ++i) {
      data_[offset_ + i] = leaves[i];
    }
    for (int i = offset_ - 1; i > 0; --i) {
      data_[i] = Monoid::op(data_[i * 2], data_[i * 2 + 1]);
    }
  }

  // Sets i-th value (0-indexed) to x.
  void set(int i, const T &x) {
    int k = offset_ + i;
    data_[k] = x;
    // Update its ancestors.
    while (k > 1) {
      k >>= 1;
      data_[k] = Monoid::op(data_[k * 2], data_[k * 2 + 1]);
    }
  }

  // Queries by [l,r) range (0-indexed, half-open interval).
  T fold(int l, int r) const {
    l = std::max(l, 0) + offset_;
    r = std::min(r, offset_) + offset_;
    T vleft = Monoid::id(), vright = Monoid::id();
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vleft = Monoid::op(vleft, data_[l++]);
      if (r & 1) vright = Monoid::op(data_[--r], vright);
    }
    return Monoid::op(vleft, vright);
  }

  T fold_all() const { return data_[1]; }

  // Returns i-th value (0-indexed).
  T operator[](int i) const { return data_[offset_ + i]; }

  friend std::ostream &operator<<(std::ostream &os, const SegTree &st) {
    os << "[";
    for (int i = 0; i < st.n(); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    return os << "]";
  }
};

struct Max {
  using T = Int;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return -1LL; }
};

using namespace std;

Int solve() {
  Int W, N;
  cin >> W >> N;
  vector<Int> dp1(W + 1, -1LL);
  dp1[0] = 0;
  for (int i = 0; i < N; ++i) {
    Int l, r, value;
    cin >> l >> r >> value;
    SegTree<Max> seg(dp1);
    for (int j = 0; j <= W; ++j) {
      Int p = max(j - r, 0LL);
      if (j - l < 0) continue;
      Int q = j - l;
      auto cur = dp1[j];
      auto ma = seg.fold(p, q + 1);
      if (ma < 0) continue;
      if (cur < ma + value) {
        dp1[j] = ma + value;
      }
    }
  }
  return dp1[W];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
