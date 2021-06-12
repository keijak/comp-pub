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
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
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
template <typename T>
struct SparseLiChaoTree {
  static_assert(std::is_arithmetic_v<T>, "must be number");

  // Identity value of the Min monoid.
  static constexpr T min_id = std::numeric_limits<T>::max();

  struct Line {
    T a, b;

    Line(T a, T b) : a(a), b(b) {}

    inline T get(T x) const { return a * x + b; }
  };

  struct Node {
    Line x;
    Node *l, *r;

    Node(const Line &x) : x{x}, l{nullptr}, r{nullptr} {}
  };

  Node *root_;
  T x_low_, x_high_;  // Domain of the lines: x_low <= x <= x_high

  SparseLiChaoTree(T x_low, T x_high)
      : root_{nullptr}, x_low_{std::move(x_low)}, x_high_{std::move(x_high)} {}

  Node *add_line(Node *t, Line &x, const T &l, const T &r, const T &x_l,
                 const T &x_r) {
    if (!t) return new Node(x);

    T t_l = t->x.get(l), t_r = t->x.get(r);

    if (t_l <= x_l && t_r <= x_r) {
      return t;
    } else if (t_l >= x_l && t_r >= x_r) {
      t->x = x;
      return t;
    } else {
      T m = (l + r) / 2;
      if (m == r) --m;
      T t_m = t->x.get(m), x_m = x.get(m);
      if (t_m > x_m) {
        std::swap(t->x, x);
        if (x_l >= t_l) {
          t->l = add_line(t->l, x, l, m, t_l, t_m);
        } else {
          t->r = add_line(t->r, x, m + 1, r, t_m + x.a, t_r);
        }
      } else {
        if (t_l >= x_l) {
          t->l = add_line(t->l, x, l, m, x_l, x_m);
        } else {
          t->r = add_line(t->r, x, m + 1, r, x_m + x.a, x_r);
        }
      }
      return t;
    }
  }

  void add_line(const T &a, const T &b) {
    Line x(a, b);
    root_ = add_line(root_, x, x_low_, x_high_, x.get(x_low_), x.get(x_high_));
  }

  Node *add_segment(Node *t, Line &x, const T &a, const T &b, const T &l,
                    const T &r, const T &x_l, const T &x_r) {
    if (r < a || b < l) return t;
    if (a <= l && r <= b) {
      Line y{x};
      return add_line(t, y, l, r, x_l, x_r);
    }
    if (t) {
      T t_l = t->x.get(l), t_r = t->x.get(r);
      if (t_l <= x_l && t_r <= x_r) return t;
    } else {
      t = new Node(Line(0, min_id));
    }
    T m = (l + r) / 2;
    if (m == r) --m;
    T x_m = x.get(m);
    t->l = add_segment(t->l, x, a, b, l, m, x_l, x_m);
    t->r = add_segment(t->r, x, a, b, m + 1, r, x_m + x.a, x_r);
    return t;
  }

  void add_segment(const T &l, const T &r, const T &a, const T &b) {
    Line x(a, b);
    root_ = add_segment(root_, x, l, r - 1, x_low_, x_high_, x.get(x_low_),
                        x.get(x_high_));
  }

  T query(const Node *t, const T &l, const T &r, const T &x) const {
    if (!t) return min_id;
    if (l == r) return t->x.get(x);
    T m = (l + r) / 2;
    if (m == r) --m;
    if (x <= m) {
      return std::min(t->x.get(x), query(t->l, l, m, x));
    } else {
      return std::min(t->x.get(x), query(t->r, m + 1, r, x));
    }
  }

  T query(const T &x) const { return query(root_, x_low_, x_high_, x); }
};

auto solve() {
  i64 n, C;
  cin >> n >> C;
  vector<i64> H(n);
  cin >> H;

  auto dp = vector(n + 1, (i64)INF);
  dp[0] = 0;

  i64 h_min = 0, h_max = i64(1e6) + 1;
  SparseLiChaoTree<i64> lct(h_min, h_max);
  lct.add_line(-2 * H[0], dp[0] + H[0] * H[0]);

  REP(i, 1, n) {
    i64 val = lct.query(H[i]);
    assert(val != INF);
    i64 h2 = H[i] * H[i];
    chmin(dp[i], val + h2 + C);
    lct.add_line(-2 * H[i], dp[i] + h2);
  }
  return dp[n - 1];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
