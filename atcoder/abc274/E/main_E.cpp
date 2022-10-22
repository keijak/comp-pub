// #define NDEBUG
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
constexpr T kBig = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;
template<typename T>
struct Point2d {
  T x;
  T y;

  Point2d() : x(0), y(0) {}
  Point2d(T x, T y) : x(x), y(y) {}
  Point2d(const Point2d &) = default;
  Point2d(Point2d &&) = default;
  Point2d &operator=(const Point2d &) = default;
  Point2d &operator=(Point2d &&) = default;
  const T &real() const { return x; }
  const T &imag() const { return y; }

  // inner product
  friend T dot(const Point2d &a, const Point2d &b) {
    return a.x * b.x + a.y * b.y;
  }

  // outer product
  friend T cross(const Point2d &a, const Point2d &b) {
    return a.x * b.y - a.y * b.x;
  }

  // |a|^2
  friend T norm(const Point2d &a) { return dot(a, a); }

  // |a|
  friend Real abs(const Point2d &a) { return std::sqrt((Real) norm(a)); }

  bool operator==(const Point2d &other) const {
    return x == other.x and y == other.y;
  }
  bool operator!=(const Point2d &other) const {
    return x != other.x or y != other.y;
  }

  Point2d &operator+=(const Point2d &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  friend Point2d operator+(const Point2d &p1, const Point2d &p2) {
    auto r = p1;
    r += p2;
    return r;
  }

  Point2d &operator-=(const Point2d &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  friend Point2d operator-(const Point2d &p1, const Point2d &p2) {
    auto r = p1;
    r -= p2;
    return r;
  }

  Point2d &operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  friend Point2d operator*(const Point2d &p, T scalar) {
    auto r = p;
    r *= scalar;
    return r;
  }
  friend Point2d operator*(T scalar, const Point2d &p) {
    auto r = p;
    r *= scalar;
    return r;
  }
  friend ostream &operator<<(ostream &os, const Point2d &p) {
    return os << "(" << p.x << ", " << p.y << ")";
  }
};
using P = Point2d<Int>;

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

auto solve() {
  int n = in, m = in;
  vector<P> cities(n), boosters(m);

  REP(i, n) {
    cin >> cities[i].x >> cities[i].y;
  }
  REP(i, m) {
    cin >> boosters[i].x >> boosters[i].y;
  }
  auto get_pt = [&](int i) -> P {
    if (i < m) return boosters[i];
    if (i < n + m) return cities[i - m];
    if (i == n + m) return P{0, 0};
    assert(false);
  };
  auto dist = [&](int i, int j, int b) -> Real {
    int c = 0;
    for (int k = 0; k < m; ++k) {
      if (has_bit(b, k)) ++c;
    }
    return abs(get_pt(i) - get_pt(j)) / pow<Real>(2, c);
  };
  auto dp = vector(1 << (n + m + 1), vector(n + m + 1, kBig<Real>));
  REP(i, n + m) {
    int b = 1 << i;
    chmin(dp[b][i], dist(n + m, i, 0));
  }
  Real ans = kBig<Real>;
  int full = 0;
  REP(i, n) {
    full |= 1 << (i + m);
  }
  REP(b, 1, 1 << (n + m + 1)) {
    REP(v, n + m + 1) {
      if (dp[b][v] == kBig<Real>) continue;
      REP(u, n + m + 1) {
        if (has_bit(b, u)) continue;
        if (u == n + m and (b & full) != full) continue;
        int nb = b | (1 << u);
        if (chmin(dp[nb][u], dp[b][v] + dist(v, u, b))) {
          DUMP(nb, u, dp[nb][u]);
          if (u == n + m) {
            chmin(ans, dp[nb][u]);
          }
        }
      }
    }
  }
//  REP(i, 1 << m) {
//    int b = i;
//    REP(j, n + 1) {
//      b |= 1 << j;
//    }
//    chmin(ans, dp[b][n + m]);
//  }
  out(ans);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
