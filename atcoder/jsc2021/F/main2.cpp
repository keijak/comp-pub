#include <bits/stdc++.h>

#include <atcoder/fenwicktree>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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

#include <boost/multiprecision/cpp_int.hpp>
using Int = boost::multiprecision::int128_t;

template <typename Monoid>
struct BroadSegTree {
  using T = typename Monoid::T;
  using i64 = long long;

 private:
  i64 n_;                            // number of valid leaves.
  i64 offset_;                       // where leaves start
  std::unordered_map<i64, T> data_;  // data size: 2*offset_

 public:
  inline i64 n() const { return n_; }
  inline i64 offset() const { return offset_; }

  explicit BroadSegTree(int n) : n_(n) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.reserve(1 << 20);
    data_.max_load_factor(0.25);
  }

  // Sets i-th value (0-indexed) to x.
  void set(i64 i, const T &x) {
    i64 k = offset_ + i;
    if (x == Monoid::id()) {
      data_.erase(k);
    } else {
      data_[k] = x;
    }
    // Update its ancestors.
    while (k > 1) {
      k >>= 1;
      const auto it0 = data_.find(k * 2);
      const auto it1 = data_.find(k * 2 + 1);
      if (it0 != data_.end() and it1 != data_.end()) {
        data_[k] = Monoid::op(it0->second, it1->second);
      } else if (it0 != data_.end()) {
        data_[k] = it0->second;
      } else if (it1 != data_.end()) {
        data_[k] = it1->second;
      } else {
        data_.erase(k);
      }
    }
  }

  // Queries by [l,r) range (0-indexed, half-open interval).
  T fold(i64 l, i64 r) const {
    l = std::max(l, 0LL) + offset_;
    r = std::min(r, offset_) + offset_;
    T vleft = Monoid::id(), vright = Monoid::id();
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) {
        if (auto it = data_.find(l++); it != data_.end()) {
          vleft = Monoid::op(vleft, it->second);
        }
      }
      if (r & 1) {
        if (auto it = data_.find(--r); it != data_.end()) {
          vright = Monoid::op(it->second, vright);
        }
      }
    }
    return Monoid::op(vleft, vright);
  }

  T fold_all() const {
    if (auto it = data_.find(1); it != data_.end()) {
      return it->second;
    } else {
      return Monoid::id();
    }
  }

  // Returns i-th value (0-indexed).
  T operator[](i64 i) const {
    if (auto it = data_.find(offset_ + i); it != data_.end()) {
      return it->second;
    } else {
      return Monoid::id();
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const BroadSegTree &st) {
    static const int kMaxOutput = 1e7;
    os << "[";
    for (int i = 0; i < min<i64>(st.n(), kMaxOutput); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    return os << "]";
  }
};

struct SumCount {
  struct T {
    long long sum;
    int count;
    bool operator==(const T &other) const {
      return sum == other.sum and count == other.count;
    }
  };
  static T op(const T &x, const T &y) {
    return {x.sum + y.sum, x.count + y.count};
  }
  static constexpr T id() { return {0, 0}; }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n, m, q;
  cin >> n >> m >> q;
  vector<array<int, 3>> queries(q);
  REP(i, q) {
    int t, x, y;
    cin >> t >> x >> y;
    --x;
    queries[i] = {t, x, y};
  }
  vector<i64> a(n, 0), b(m, 0);
  BroadSegTree<SumCount> sa(1'0000'0005), sb(1'0000'0005);
  sa.set(0, {0LL, n});
  sb.set(0, {0LL, m});

  Int ans1 = 0, ans2 = 0;
  REP(i, q) {
    auto [t, x, y] = queries[i];
    if (t == 1 and a[x] != y) {
      i64 p = a[x];
      auto pl = sb.fold(0, p).count;
      auto yl = sb.fold(0, y).count;
      ans1 -= pl * Int(p);
      ans1 += yl * Int(y);
      if (p < y) {
        ans1 -= sb.fold(p + 1, y + 1).sum;
      } else {
        ans1 += sb.fold(y + 1, p + 1).sum;
      }
      ans2 -= sb[p].sum;
      ans2 += sb[y].sum;
      auto pv = sa[p];
      auto yv = sa[y];
      sa.set(p, {pv.sum - p, pv.count - 1});
      sa.set(y, {yv.sum + y, yv.count + 1});
      a[x] = y;
    } else if (t == 2 and b[x] != y) {
      i64 p = b[x];
      i64 pl = sa.fold(0, p).count;
      i64 yl = sa.fold(0, y).count;
      ans1 -= pl * p;
      ans1 += yl * y;
      if (p < y) {
        ans1 -= sa.fold(p + 1, y + 1).sum;
      } else {
        ans1 += sa.fold(y + 1, p + 1).sum;
      }
      ans2 -= sa.fold(p, p + 1).sum;
      ans2 += sa.fold(y, y + 1).sum;
      auto pv = sb[p];
      auto yv = sb[y];
      sb.set(p, {pv.sum - p, pv.count - 1});
      sb.set(y, {yv.sum + y, yv.count + 1});
      b[x] = y;
    }
    Int ans = ans1 + ans2;
    cout << ans << "\n";
  }
}
