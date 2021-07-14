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
inline int ssize(const T &a) {
  return (int)std::size(a);
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
                          is_iterable<T>::value and
                          !std::is_same<T, std::string_view>::value and
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template <int sign = 1>
struct Infinity {
  template <typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned_v<T>,
                  "must be positive in an unsigned type");
    if constexpr (std::numeric_limits<T>::has_infinity) {
      return T(sign) * std::numeric_limits<T>::infinity();
    } else {
      return T(sign) * (std::numeric_limits<T>::max() / T(2));
    }
  }
  constexpr Infinity<sign * -1> operator-() const { return {}; }
  template <typename T>
  friend bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template <typename T>
  friend bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
constexpr Infinity<> INF;

// Sparse (dynamic) Li-Chao Tree using gp_hash_table.

struct Min {
  using T = long long;
  static bool less(const T &x, const T &y) { return x < y; }
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

// Line-like function.
//
// Not necessarily a line, but must hold the transcending property:
// Two curves cross at most once in the specified range.
template <class T>
struct Line {
  T a;
  T b;

  Line(T a, T b) : a(std::move(a)), b(std::move(b)) {}

  T eval(T x) const { return a * x + b; }

  static Line constant(T value) { return Line(0, value); }
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

auto solve() {
  i64 n, C;
  cin >> n >> C;
  vector<i64> H(n);
  cin >> H;

  auto dp = vector(n + 1, (i64)INF);
  dp[0] = 0;

  LiChaoTree<Min> lct(H);
  lct.add_line(Line<i64>(-2 * H[0], dp[0] + H[0] * H[0]));

  REP(i, 1, n) {
    i64 val = lct.query(H[i]);
    i64 h2 = H[i] * H[i];
    chmin(dp[i], val + h2 + C);
    lct.add_line(Line<i64>(-2 * H[i], dp[i] + h2));
  }
  return dp[n - 1];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
