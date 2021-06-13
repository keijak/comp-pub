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

// Li-Chao Tree
// - Finds min values.
// - Dynamically creates nodes.
template <typename T>
struct SparseLiChaoTree {
  static_assert(std::is_arithmetic_v<T>, "must be number");

  // Identity value of the Min monoid.
  static constexpr T min_id = std::numeric_limits<T>::max();

  struct Line {
    T a, b;

    Line(T a, T b) : a(a), b(b) {}

    inline T eval(T x) const { return a * x + b; }
  };

  struct Node {
    Line f;
    Node *l, *r;

    Node(Line f) : f{std::move(f)}, l{nullptr}, r{nullptr} {}
  };

  Node *root_;
  T x_low_, x_high_;  // Domain of the lines: x_low <= x <= x_high

  SparseLiChaoTree(T x_low, T x_high)
      : root_{nullptr}, x_low_{std::move(x_low)}, x_high_{std::move(x_high)} {}

  void add_line(const T &a, const T &b) {
    Line f(a, b);
    root_ =
        add_line(root_, f, x_low_, x_high_, f.eval(x_low_), f.eval(x_high_));
  }
  void add_segment(const T &l, const T &r, const T &a, const T &b) {
    Line f(a, b);
    root_ = add_segment(root_, f, l, r - 1, x_low_, x_high_, f.eval(x_low_),
                        f.eval(x_high_));
  }

  T query(const T &x) const { return query(root_, x_low_, x_high_, x); }

 private:
  Node *add_line(Node *t, Line &f, const T &l, const T &r, const T &f_l,
                 const T &f_r) {
    if (!t) return new Node(f);

    T t_l = t->f.eval(l), t_r = t->f.eval(r);

    if (t_l <= f_l && t_r <= f_r) {
      return t;
    } else if (t_l >= f_l && t_r >= f_r) {
      t->f = f;
      return t;
    } else {
      T m = (l + r) / 2;
      if (m == r) --m;
      T t_m = t->f.eval(m), f_m = f.eval(m);
      if (t_m > f_m) {
        std::swap(t->f, f);
        if (f_l >= t_l) {
          t->l = add_line(t->l, f, l, m, t_l, t_m);
        } else {
          t->r = add_line(t->r, f, m + 1, r, t_m + f.a, t_r);
        }
      } else {
        if (t_l >= f_l) {
          t->l = add_line(t->l, f, l, m, f_l, f_m);
        } else {
          t->r = add_line(t->r, f, m + 1, r, f_m + f.a, f_r);
        }
      }
      return t;
    }
  }

  Node *add_segment(Node *t, Line &f, const T &a, const T &b, const T &l,
                    const T &r, const T &f_l, const T &f_r) {
    if (r < a || b < l) return t;
    if (a <= l && r <= b) {
      Line f2{f};
      return add_line(t, f2, l, r, f_l, f_r);
    }
    if (t) {
      T t_l = t->f.eval(l), t_r = t->f.eval(r);
      if (t_l <= f_l && t_r <= f_r) return t;
    } else {
      t = new Node(Line(0, min_id));
    }
    T m = (l + r) / 2;
    if (m == r) --m;
    T f_m = f.eval(m);
    t->l = add_segment(t->l, f, a, b, l, m, f_l, f_m);
    t->r = add_segment(t->r, f, a, b, m + 1, r, f_m + f.a, f_r);
    return t;
  }

  // l <= x <= r
  T query(const Node *t, const T &l, const T &r, const T &x) const {
    assert(l <= x and x <= r);
    if (!t) return min_id;
    if (l == r) return t->f.eval(x);
    T m = (l + r) / 2;
    if (m == r) --m;
    if (x <= m) {
      return std::min(t->f.eval(x), query(t->l, l, m, x));
    } else {
      return std::min(t->f.eval(x), query(t->r, m + 1, r, x));
    }
  }
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
