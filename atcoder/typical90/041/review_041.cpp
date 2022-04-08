#include <atcoder/math>
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
void exit_() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<typename T>  // T: int, double, etc.
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
  friend T dot(const Point2d &a, const Point2d &b) { return a.x * b.x + a.y * b.y; }

  // outer product
  friend T cross(const Point2d &a, const Point2d &b) { return a.x * b.y - a.y * b.x; }

  T abs2() const { return dot(*this, *this); }               // |x|^2
  double abs() const { return std::sqrt((double) abs2()); }  // |x|

  Point2d &operator+=(const Point2d &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  friend Point2d operator+(const Point2d &p1, const Point2d &p2) {
    return (Point2d(p1) += p2);
  }

  Point2d &operator-=(const Point2d &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  friend Point2d operator-(const Point2d &p1, const Point2d &p2) {
    return (Point2d(p1) -= p2);
  }

  Point2d &operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  friend Point2d operator*(const Point2d &p, T scalar) {
    return (Point2d(p) *= scalar);
  }
  friend Point2d operator*(T scalar, const Point2d &p) {
    return (Point2d(p) *= scalar);
  }
};
using P = Point2d<long long>;

std::pair<std::vector<P>, std::vector<P>> scan_convex_hull(std::vector<P> ps) {
  std::sort(ps.begin(), ps.end(), [](const P &a, const P &b) {
    return std::tie(a.real(), a.imag()) < std::tie(b.real(), b.imag());
  });
  std::vector<P> lower, upper;
  for (int i = 0; i < (int) ps.size(); ++i) {
    auto ax = ps[i].x, ay = ps[i].y;
    P now{ax, ay};
    while (lower.size() >= 2) {
      P &p2 = lower[lower.size() - 2];
      P v1 = lower.back() - p2;
      P v2 = now - p2;
      if (cross(v1, v2) > 0) break;
      lower.pop_back();
    }
    lower.push_back(move(now));
  }
  for (int i = ps.size() - 1; i >= 0; --i) {
    auto ax = ps[i].x, ay = ps[i].y;
    P now{ax, ay};
    while (upper.size() >= 2) {
      P &p2 = upper[upper.size() - 2];
      P v1 = upper.back() - p2;
      P v2 = now - p2;
      if (cross(v1, v2) > 0) break;
      upper.pop_back();
    }
    upper.push_back(move(now));
  }
  reverse(upper.begin(), upper.end());
  return {lower, upper};
}

auto solve() {
  const int n = in;
  vector<P> points(n);
  REP(i, n) {
    Real x = in, y = in;
    points[i] = P(x, y);
  }

  auto[lower, upper] = scan_convex_hull(points);
  Int area2 = 0;
  Int num_on_corner = ssize(lower) + ssize(upper) - 2;
  Int num_on_border = num_on_corner;
  for (int i = 1; i < ssize(upper); ++i) {
    auto v1 = upper[i] - upper[0];
    auto v2 = upper[i - 1] - upper[0];
    auto v3 = v2 - v1;
    area2 += ((v1.x) - (v2.x)) * ((v1.y) + (v2.y));
    num_on_border += gcd((abs(v3.x)), (abs(v3.y))) - 1;
  }
  for (int i = 1; i < ssize(lower); ++i) {
    auto v1 = lower[i] - lower[0];
    auto v2 = lower[i - 1] - lower[0];
    auto v3 = v2 - v1;
    area2 -= ((v1.x) - (v2.x)) * ((v1.y) + (v2.y));
    num_on_border += gcd((abs(v3.x)), (abs(v3.y))) - 1;
  }
  Int num_inside = ((area2) - num_on_border + 2);
  assert(num_inside % 2 == 0);
  num_inside /= 2;
  Int ans = num_inside + num_on_border - n;
  print(ans);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}
