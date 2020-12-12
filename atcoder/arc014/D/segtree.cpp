#include <bits/stdc++.h>

#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
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

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}

template <typename T>
struct Compress {
  std::vector<T> vec;

  explicit Compress(std::vector<T> v) : vec(v) {
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
  }

  int size() const { return vec.size(); }

  int index(T x) const {
    return lower_bound(vec.begin(), vec.end(), x) - vec.begin();
  }

  const T &value(int i) const { return vec[i]; }
};

template <typename Monoid>
struct SegTree {
  using T = typename Monoid::T;

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

 private:
  int n_;                // number of valid leaves.
  int offset_;           // where leaves start
  std::vector<T> data_;  // data size: 2*offset_
};

struct Sum2 {
  struct T {
    int sum;
    int width;
  };

  // Fold: Sum
  static T op(const T &x, const T &y) {
    return {x.sum + y.sum, x.width + y.width};
  }
  static constexpr T id() { return {0, 0}; }
};

using namespace std;

void solve() {
  int K, N, M;
  cin >> K >> N >> M;
  vector<int> L(N), gap(N - 1);
  cin >> L;
  REP(i, N - 1) { gap[i] = L[i + 1] - L[i] - 1; }
  Compress<int> compress(gap);
  SegTree<Sum2> seg(compress.size());
  REP(i, N - 1) {
    int j = compress.index(gap[i]);
    auto v = seg[j];
    seg.set(j, {v.sum + gap[i], v.width + 1});
  }
  REP(qi, M) {
    int x, y;
    cin >> x >> y;
    int xy = x + y;
    int j = compress.index(xy);
    auto v = seg.fold(0, j);
    int over_cnt = v.width;
    int over_sum = v.sum;
    int res = over_sum + (N - 1 - over_cnt) * xy;
    int first = min(L[0] - 1, x);
    int last = min(K - L[N - 1], y);
    res += first + last + N;
    cout << res << '\n';
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
