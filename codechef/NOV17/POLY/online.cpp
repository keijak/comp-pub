#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// Line-like function.
//
// Not necessarily a line, but must hold the transcending property:
// Two curves cross at most once in the specified range.
template <class T>
struct Line {
  array<T, 4> a;

  explicit Line(array<T, 4> a) : a(move(a)) {}

  T eval(T x) const {
    return a[0] + a[1] * x + a[2] * x * x + a[3] * x * x * x;
  }

  static Line constant(T value) { return Line(array<T, 4>{value, 0, 0, 0}); }
};

template <class Op>  // Min or Max
struct SparseLiChaoTree {
  using T = typename Op::T;
  static_assert(std::is_arithmetic<T>::value, "must be number");

  // Creates a Li-Chao Tree with query candidates it can answer.
  explicit SparseLiChaoTree(i64 x_low, i64 x_high)
      : x_low_(x_low), n_(x_high - x_low) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    lines_.reserve(1 << 20);
    lines_.max_load_factor(0.25);
  }

  // Adds y = g(x).
  void add_line(Line<T> g) { update(std::move(g), 0, n_); }

  // Adds y = g(x) in xl <= x < xr.
  void add_segment(Line<T> g, i64 xl, i64 xr) {
    i64 l = xl - x_low_;
    i64 r = xr - x_low_;
    update(std::move(g), l, r);
  }

  // Returns the minimum/maximum f(x) at x.
  T query(T x) const {
    assert(x < n_);
    T y = Op::id();
    for (i64 i = x - x_low_ + offset_; i > 0; i >>= 1) {
      auto it = lines_.find(i);
      if (it != lines_.end()) {
        y = Op::op(y, it->second.eval(x));
      }
    }
    return y;
  }

 private:
  void update(Line<T> g, i64 l, i64 r) {
    for (l += offset_, r += offset_; l < r; l >>= 1, r >>= 1) {
      if (l & 1) descend(g, l++);
      if (r & 1) descend(g, --r);
    }
  }

  void descend(Line<T> g, i64 i) {
    int l = i, r = i + 1;
    while (l < offset_) {
      l <<= 1;
      r <<= 1;
    }
    while (l < r) {
      auto c = (l + r) >> 1;
      auto xl = l - offset_ + x_low_;
      auto xc = c - offset_ + x_low_;
      auto xr = r - 1 - offset_ + x_low_;
      auto fit = lines_.find(i);
      if (fit == lines_.end()) {
        lines_.emplace(i, std::move(g));
        return;
      }
      Line<T> &f = fit->second;
      if (not Op::less(g.eval(xl), f.eval(xl)) and
          not Op::less(g.eval(xr), f.eval(xr))) {
        return;
      }
      if (not Op::less(f.eval(xl), g.eval(xl)) and
          not Op::less(f.eval(xr), g.eval(xr))) {
        f = std::move(g);
        return;
      }
      if (Op::less(g.eval(xc), f.eval(xc))) {
        std::swap(f, g);
      }
      if (Op::less(g.eval(xl), f.eval(xl))) {
        i = (i << 1) | 0;
        r = c;
      } else {
        i = (i << 1) | 1;
        l = c;
      }
    }
  }

  i64 x_low_;
  i64 n_;
  i64 offset_;
  std::vector<T> xs_;
  std::unordered_map<i64, Line<T>> lines_;
};

struct Min {
  using T = i64;
  static bool less(const T &x, const T &y) { return x < y; }
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

using LCT = SparseLiChaoTree<Min>;

void solve() {
  int n;
  cin >> n;

  LCT lct(0, 100005);
  vector<Line<i64>> polys;
  REP(i, n) {
    array<i64, 4> a;
    REP(j, 4) cin >> a[j];
    Line<i64> f(a);
    polys.push_back(f);
    lct.add_segment(f, 1005, 100005);
  }

  vector<i64> early(1005, Min::id());
  REP(j, early.size()) {
    REP(i, n) { chmin(early[j], polys[i].eval(j)); }
  }

  int q;
  cin >> q;
  vector<i64> ans(q);
  REP(i, q) {
    i64 t;
    cin >> t;
    if (t < (int)(early.size())) {
      ans[i] = early[t];
    } else {
      ans[i] = lct.query(t);
    }
  }
  REP(i, q) { cout << ans[i] << "\n"; }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) solve();
}
