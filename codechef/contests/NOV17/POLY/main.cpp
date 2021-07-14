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

#ifdef MY_DEBUG
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
struct LiChaoTree {
  using T = typename Op::T;
  static_assert(std::is_arithmetic<T>::value, "must be number");

  // Creates a Li-Chao Tree with query candidates it can answer.
  explicit LiChaoTree(const std::vector<T> &xs) : n_(0), xs_(xs) {
    std::sort(xs_.begin(), xs_.end());
    xs_.erase(std::unique(xs_.begin(), xs_.end()), xs_.end());
    n_ = (int)xs_.size();
    lines_.assign(n_ << 1, Line<T>::constant(Op::id()));
  }

  // Adds y = g(x).
  void add_line(Line<T> g) { update(std::move(g), 0, n_); }

  // Adds y = g(x) in xl <= x < xr.
  void add_segment(Line<T> g, T xl, T xr) {
    int l = std::lower_bound(xs_.begin(), xs_.end(), xl) - xs_.begin();
    int r = std::lower_bound(xs_.begin(), xs_.end(), xr) - xs_.begin();
    update(std::move(g), l, r);
  }

  // Returns the minimum/maximum f(x) at x.
  T query(T x) const {
    int i = std::lower_bound(xs_.begin(), xs_.end(), x) - xs_.begin();
    assert(i != n_ && x == xs_[i]);
    T y = Op::id();
    for (i += n_; i > 0; i >>= 1) y = Op::op(y, lines_[i].eval(x));
    return y;
  }

 private:
  void update(Line<T> g, int l, int r) {
    for (l += n_, r += n_; l < r; l >>= 1, r >>= 1) {
      if (l & 1) descend(g, l++);
      if (r & 1) descend(g, --r);
    }
  }

  void descend(Line<T> g, int i) {
    int l = i, r = i + 1;
    while (l < n_) {
      l <<= 1;
      r <<= 1;
    }
    while (l < r) {
      int c = (l + r) >> 1;
      T xl = xs_[l - n_];
      T xc = xs_[c - n_];
      T xr = xs_[r - 1 - n_];
      Line<T> &f = lines_[i];
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

  int n_;
  std::vector<T> xs_;
  std::vector<Line<T>> lines_;
};

struct Min {
  using T = long long;
  static bool less(const T &x, const T &y) { return x < y; }
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

struct Max {
  using T = long long;
  static bool less(const T &x, const T &y) { return x > y; }
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::lowest(); }
};

using LCT = LiChaoTree<Min>;

void solve() {
  int n;
  cin >> n;

  vector<Line<i64>> polys;
  REP(i, n) {
    array<i64, 4> a;
    REP(j, 4) cin >> a[j];
    Line<i64> f(a);
    polys.push_back(f);
  }

  vector<i64> early(1005, Min::id());
  REP(j, early.size()) {
    REP(i, n) { chmin(early[j], polys[i].eval(j)); }
  }

  int q;
  cin >> q;
  vector<i64> ts;
  vector<i64> ans(q);
  vector<pair<int, i64>> offline_queries;
  REP(i, q) {
    i64 t;
    cin >> t;
    if (t < (int)(early.size())) {
      ans[i] = early[t];
    } else {
      ts.push_back(t);
      offline_queries.emplace_back(i, t);
    }
  }
  LCT lct(move(ts));
  for (const auto &f : polys) {
    lct.add_segment(f, 1005, 100005);
  }
  for (auto q : offline_queries) {
    ans[q.first] = lct.query(q.second);
  }
  REP(i, q) { cout << ans[i] << "\n"; }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) solve();
}
