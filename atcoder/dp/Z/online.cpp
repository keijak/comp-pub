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

#ifdef ENABLE_DEBUG
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

#include <ext/pb_ds/assoc_container.hpp>

template <class K, class V>
using gp_hash_table = __gnu_pbds::gp_hash_table<
    K, V, std::hash<K>, std::equal_to<K>,
    __gnu_pbds::direct_mask_range_hashing<K>, __gnu_pbds::linear_probe_fn<>,
    __gnu_pbds::hash_standard_resize_policy<
        __gnu_pbds::hash_exponential_size_policy<>,
        __gnu_pbds::hash_load_check_resize_trigger<true>, true>>;

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
struct SparseLiChaoTree {
  using Y = typename Op::T;  // y-coordinate
  using X = long long;       // x-coordinate
  static_assert(std::is_arithmetic<Y>::value, "must be number");
  static_assert(std::is_integral<X>::value, "must be integer");

  // Creates a Li-Chao Tree with x-coordinate bounds [x_low, x_high).
  // Dynamically creates nodes.
  SparseLiChaoTree(X x_low, X x_high) : x_low_(x_low), n_(x_high - x_low) {
    assert(x_low <= x_high);
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    lines_.resize(1 << 20);
  }

  // Adds y = g(x).
  void add_line(Line<Y> g) { update(std::move(g), 0, n_); }

  // Adds y = g(x) in xl <= x < xr.
  void add_segment(Line<Y> g, X xl, X xr) {
    auto l = xl - x_low_;
    auto r = xr - x_low_;
    update(std::move(g), l, r);
  }

  // Returns the minimum/maximum f(x) at x.
  Y query(X x) const {
    assert(x < n_);
    Y y = Op::id();
    for (X i = x - x_low_ + offset_; i > 0; i >>= 1) {
      auto it = lines_.find(i);
      if (it != lines_.end()) {
        y = Op::op(y, it->second.eval(x));
      }
    }
    return y;
  }

 private:
  void update(Line<Y> g, X l, X r) {
    for (l += offset_, r += offset_; l < r; l >>= 1, r >>= 1) {
      if (l & 1) descend(g, l++);
      if (r & 1) descend(g, --r);
    }
  }

  void descend(Line<Y> g, X i) {
    X l = i, r = i + 1;
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
        lines_.insert({i, std::move(g)});
        return;
      }
      Line<Y> &f = fit->second;
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

  X x_low_, n_, offset_;
  gp_hash_table<X, Line<Y>> lines_;
};

auto solve() {
  i64 n, C;
  cin >> n >> C;
  vector<i64> H(n);
  cin >> H;

  auto dp = vector(n + 1, (i64)INF);
  dp[0] = 0;

  i64 h_min = 0, h_max = i64(1e6) + 1;
  SparseLiChaoTree<Min> lct(h_min, h_max);
  lct.add_line(Line<i64>(-2 * H[0], dp[0] + H[0] * H[0]));

  REP(i, 1, n) {
    i64 val = lct.query(H[i]);
    assert(val != INF);
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
