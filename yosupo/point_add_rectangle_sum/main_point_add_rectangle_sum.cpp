#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<typename Monoid>
struct SegmentTree {
  using T = typename Monoid::T;

  int n_;                // number of valid leaves.
  int offset_;           // where leaves start
  std::vector<T> data_;  // data size: 2*offset_

  inline int size() const { return n_; }
  inline int offset() const { return offset_; }

  explicit SegmentTree(int n) : n_(n) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
  }

  explicit SegmentTree(const std::vector<T> &leaves) : n_(leaves.size()) {
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

  friend std::ostream &operator<<(std::ostream &os, const SegmentTree &st) {
    os << "[";
    for (int i = 0; i < st.size(); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    return os << "]";
  }
};

template<typename Monoid>
struct SegmentTree2d {
  using T = typename Monoid::T;
  int offset_;
  vector<SegmentTree<Monoid>> segs_;

  // [row_id][col_id] => (y, x)
  vector<vector<pair<Int, Int>>> yx_;
  // [(x, y)] sorted in ascending order
  vector<pair<Int, Int>> xy_;

  // [(x,y) => value]
  explicit SegmentTree2d(const map<pair<Int, Int>, T> &data)
      : offset_(1), xy_(data.size()) {
    const int n = (int) data.size();  // number of data points
    while (offset_ < n) offset_ *= 2;

    vector<const pair<Int, Int> *> coords(n);
    vector<const T *> vals(n);
    {
      int i = 0;
      for (auto it = data.begin(); it != data.end(); ++it, ++i) {
        coords[i] = &it->first;
        vals[i] = &it->second;
        // Temporarily fill xy_ with (x, coord_id).
        xy_[i] = {it->first.first, i};
      }
    }

    std::sort(ALL(xy_),
              [&](const auto &p1, const auto &p2) {  // Compare by X, then Y.
                return std::tie(p1.first, coords[p1.second]->second) <
                    std::tie(p2.first, coords[p2.second]->second);
              });
    yx_.resize(2 * offset_ - 1);
    segs_.resize(2 * offset_ - 1, SegmentTree<Monoid>(0));
    // Build leaves.
    for (int i = 0; i < n; ++i) {
      // Temporarily fill yx_ with (coord_id, x).
      yx_[i + offset_ - 1] = {{xy_[i].second, xy_[i].first}};
      vector<T> row_data = {*vals[xy_[i].second]};
      segs_[i + offset_ - 1] = SegmentTree<Monoid>{std::move(row_data)};
      // Set the second elements of xy_ to y.
      xy_[i].second = coords[xy_[i].second]->second;
    }
    // Build inner nodes.
    for (int i = offset_ - 2; i >= 0; --i) {
      yx_[i].resize(yx_[2 * i + 1].size() + yx_[2 * i + 2].size());
      if (yx_[i].empty()) continue;
      std::merge(ALL(yx_[2 * i + 1]), ALL(yx_[2 * i + 2]), yx_[i].begin(),
                 [&](const auto &p1, const auto &p2) {
                   return std::tie(coords[p1.first]->second, p1.second) <
                       std::tie(coords[p2.first]->second, p2.second);
                 });
      vector<T> row_data(yx_[i].size());
      for (int j = 0; j < (int) yx_[i].size(); ++j) {
        row_data[j] = *vals[yx_[i][j].first];
      }
      segs_[i] = SegmentTree<Monoid>{std::move(row_data)};
    }

    // Set the first elements of yx_ to y.
    for (int i = 0; i < 2 * offset_ - 1; ++i) {
      for (auto &[y, x]: yx_[i]) {
        y = coords[y]->second;
      }
    }
  }

  // Query a rectangle: [x_lo, x_hi) x [y_lo, y_hi).
  T fold(Int x_lo, Int x_hi, Int y_lo, Int y_hi) const {
    const int xid_lo =
        std::lower_bound(ALL(xy_),
                         pair{x_lo, std::numeric_limits<Int>::min()}) -
            xy_.begin();
    const int xid_hi =
        std::lower_bound(ALL(xy_),
                         pair{x_hi, std::numeric_limits<Int>::min()}) -
            xy_.begin();
    if (xid_lo >= xid_hi) return Monoid::id();
    return fold_x(xid_lo, xid_hi, y_lo, y_hi, 0, 0, offset_);
  }

  T get(Int x, Int y) const {
    auto key = std::pair(x, y);
    auto it = std::lower_bound(ALL(xy_), key);
    if (it == xy_.end() or *it != key) return Monoid::id();
    int xid = int(it - xy_.begin()) + offset_ - 1;
    int yid = std::lower_bound(ALL(yx_[xid]), pair(y, x)) - yx_[xid].begin();
    return segs_[xid][yid];
  }

  // Requirement: (x, y) must be one of existing points.
  // Cannot insert a new point.
  void set(Int x, Int y, T val) {
    auto key = std::pair(x, y);
    auto it = std::lower_bound(ALL(xy_), key);
    assert(it != xy_.end() and *it == key);  // must be found
    int xid = it - xy_.begin();
    return set_x(xid, x, y, std::move(val));
  }

 private:
  T fold_x(int xid_lo, int xid_hi, Int y_lo, Int y_hi, int k, int nxid_lo,
           int nxid_hi) const {
    if (nxid_hi <= xid_lo or xid_hi <= nxid_lo) return Monoid::id();
    if (xid_lo <= nxid_lo and nxid_hi <= xid_hi) {
      const int yid_lo =
          lower_bound(ALL(yx_[k]),
                      pair{y_lo, std::numeric_limits<Int>::min()}) -
              yx_[k].begin();
      const int yid_hi =
          lower_bound(ALL(yx_[k]),
                      pair{y_hi, std::numeric_limits<Int>::min()}) -
              yx_[k].begin();
      return (yid_lo >= yid_hi) ? Monoid::id() : segs_[k].fold(yid_lo, yid_hi);
    } else {
      int m = (nxid_lo + nxid_hi) >> 1;
      return Monoid::op(
          fold_x(xid_lo, xid_hi, y_lo, y_hi, 2 * k + 1, nxid_lo, m),
          fold_x(xid_lo, xid_hi, y_lo, y_hi, 2 * k + 2, m, nxid_hi));
    }
  }

  void set_x(int xid, Int x, Int y, T val) {
    xid += offset_ - 1;
    int yid = std::lower_bound(ALL(yx_[xid]), pair(y, x)) - yx_[xid].begin();
    segs_[xid].set(yid, val);
    while (xid > 0) {
      xid = (xid - 1) / 2;
      yid = std::lower_bound(ALL(yx_[xid]), pair(y, x)) - yx_[xid].begin();
      segs_[xid].set(yid, val);
    }
  }
};

struct SumOp {
  using T = long long;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int n = in, Q = in;
  map<pair<Int, Int>, Int> data;
  vector<array<Int, 5>> queries;
  REP(i, n) {
    Int x = in, y = in, w = in;
    data[{x, y}] += w;
  }
  REP(i, Q) {
    int qt = in;
    if (qt == 0) {
      Int x = in, y = in, w = in;
      data[{x, y}] = data[{x, y}];
      queries.push_back({0, x, y, w});
    } else {
      Int l = in, d = in, r = in, u = in;
      queries.push_back({1, l, d, r, u});
    }
  }
  SegmentTree2d<SumOp> st(data);

  for (auto &q: queries) {
    int qt = q[0];
    if (qt == 0) {
      Int x = q[1], y = q[2], w = q[3];
      st.set(x, y, st.get(x, y) + w);
    } else {
      Int l = q[1], d = q[2], r = q[3], u = q[4];
      print(st.fold(l, r, d, u));
    }
  }
}
