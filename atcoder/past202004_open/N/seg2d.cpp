#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
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
#define cerr if(false)std::cerr
#endif

using namespace std;

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(move(v)) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  bool contains(const T &x) const {
    return std::binary_search(values.begin(), values.end(), x);
  }
};

template<typename Monoid>
struct SegmentTree {
  using T = typename Monoid::T;

  int n_;                // number of valid leaves.
  int offset_;           // where leaves start
  std::vector<T> data_;  // data size: 2*offset_

  inline int size() const { return n_; }
  inline int offset() const { return offset_; }

  explicit SegmentTree(int n) : n_(n), offset_(1) {
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
struct StaticSegmentTree2d {
  using T = typename Monoid::T;
  int offset_;
  vector<SegmentTree<Monoid>> segs_;

  // [row_id][col_id] => (y, x)
  vector<vector<pair<Int, Int>>> yx_;
  // [(x, y)] sorted in ascending order
  vector<pair<Int, Int>> xy_;

  // [(x,y) => value]
  explicit StaticSegmentTree2d(const map<pair<Int, Int>, T> &data)
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

  // Query a rectangle: top_left x bottom_right.
  T fold(const Int tlx, const Int tly, const Int brx, const Int bry) {
    const int qtid =
        lower_bound(ALL(xy_), pair{tlx, numeric_limits<Int>::min()}) -
            xy_.begin();
    const int qbid =
        upper_bound(ALL(xy_), pair{brx, numeric_limits<Int>::min()}) -
            xy_.begin();
    return (qtid >= qbid) ? Monoid::id()
                          : fold_x(qtid, qbid, tly, bry, 0, 0, offset_);
  }

  T get(Int x, Int y) { return fold(x, y, x + 1, y + 1); }

  void set(Int x, Int y, T val) {
    int id = lower_bound(ALL(xy_), pair(x, y)) - xy_.begin();
    return set_x(id, x, y, std::move(val));
  }

 private:
  T fold_x(const int qtid, const int qbid, const Int ql, const Int qr,
           const int k, const int ntid, const int nbid) {
    if (nbid <= qtid or qbid <= ntid) return Monoid::id();
    if (qtid <= ntid and nbid <= qbid) {
      const int qlid =
          lower_bound(ALL(yx_[k]), pair{ql, numeric_limits<Int>::min()}) -
              yx_[k].begin();
      const int qrid =
          upper_bound(ALL(yx_[k]), pair{qr, numeric_limits<Int>::min()}) -
              yx_[k].begin();
      return (qlid >= qrid) ? Monoid::id() : segs_[k].fold(qlid, qrid);
    } else {
      int m = (ntid + nbid) >> 1;
      return Monoid::op(fold_x(qtid, qbid, ql, qr, 2 * k + 1, ntid, m),
                        fold_x(qtid, qbid, ql, qr, 2 * k + 2, m, nbid));
    }
  }

  void set_x(int id, Int x, Int y, T val) {
    id += offset_ - 1;
    int yid = lower_bound(ALL(yx_[id]), pair(y, x)) - yx_[id].begin();
    segs_[id].set(yid, val);
    while (id > 0) {
      id = (id - 1) / 2;
      yid = lower_bound(ALL(yx_[id]), pair(y, x)) - yx_[id].begin();
      segs_[id].set(yid, val);
    }
  }
};

struct SumOp {
  using T = long long;
  static T op(const T &x, const T &y) {
    return x + y;
    // alt: saturating_add(x, y)
  }
  static constexpr T id() { return 0; }
  static T invert(const T &x) { return -x; }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n = in, Q = in;
  map<pair<Int, Int>, Int> data;
  REP(i, n) {
    Int x1 = in, y1 = in, d = in, c = in;
    Int x2 = x1 + d + 1;
    Int y2 = y1 + d + 1;
    data[{x1, y1}] += c;
    data[{x1, y2}] -= c;
    data[{x2, y1}] -= c;
    data[{x2, y2}] += c;
  }
  StaticSegmentTree2d<SumOp> segs(data);
  REP(i, Q) {
    Int a = in, b = in;
    auto res = segs.fold(-5e9, -5e9, a + 1, b + 1);
    print(res);
  }
}
