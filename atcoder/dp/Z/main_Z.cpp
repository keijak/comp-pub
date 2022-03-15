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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
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
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
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

struct Min {
  using T = long long;
  static bool less(const T &x, const T &y) { return x < y; }
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

// Line: ax + b
template<class T>
struct Line {
  T a;
  T b;

  Line(T a, T b) : a(std::move(a)), b(std::move(b)) {}

  T eval(T x) const { return a * x + b; }

  static Line constant(T value) { return Line(0, value); }
};

template<class Op>  // Min or Max
struct LiChaoTree {
  using T = typename Op::T;
  static_assert(std::is_arithmetic<T>::value, "must be number");

  // Creates a Li-Chao Tree with query candidates it can answer.
  explicit LiChaoTree(const std::vector<T> &xs) : n_(0), xs_(xs) {
    std::sort(xs_.begin(), xs_.end());
    xs_.erase(std::unique(xs_.begin(), xs_.end()), xs_.end());
    n_ = (int) xs_.size();
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
  int n = in;
  Int C = in;
  vector<Int> H = in(n);
  LiChaoTree<Min> lct(H);
  auto dp = vector(n + 1, kBigVal<Int>);
  dp[0] = 0;
  lct.add_line(Line(-2 * H[0], H[0] * H[0]));
  REP(i, 1, n) {
    Int minval = lct.query(H[i]);
    dp[i] = H[i] * H[i] + C + minval;
    Line line(-2 * H[i], H[i] * H[i] + dp[i]);
    lct.add_line(line);
  }
  print(dp[n - 1]);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    (solve());
  }
}
