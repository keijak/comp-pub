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
using P = Point2d<Int>;

auto solve() {
  const int n = in;
  vector<P> polygon(n);
  Int ax = 0, ay = 0;
  REP(i, n) {
    Int x = in, y = in;
    polygon[i] = P(x, y);
    ax += x;
    ay += y;
  }
  Real gx = Real(ax) / n, gy = Real(ay) / n;
  const int m = in;
  vector<P> mov(m);
  REP(i, m) {
    int u = in, v = in;
    mov[i] = P(u, v);
  }
  vector<bool> inside(n, false);
  vector<int> worst_move(n, -1);
  REP(i, n) {
    const P &p1 = polygon[i];
    const P &p2 = polygon[(i + 1) % n];
    const P v = p2 - p1;
    // cross(p2-p1, g-p1)
    Real cp = Real(v.x) * (gy - p1.y) - Real(v.y) * (gx - p1.x);
    inside[i] = cp >= 0;
    P ho(v.y, -v.x);
    if ((cross(v, ho) >= 0) != inside[i]) {
      ho = P(-v.y, v.x);
      assert((cross(v, ho) >= 0) == inside[i]);
    }
    Int max_dot_prod = -kBigVal<Int>;
    int max_j = -1;
    REP(j, m) {
      Int dp = dot(mov[j], ho);
      if (chmax(max_dot_prod, dp)) {
        max_j = j;
      }
    }
    assert(max_j != -1);
    worst_move[i] = max_j;
  }

  const int Q = in;
  REP(qi, Q) {
    Int a = in, b = in;
    P qp(a, b);
    bool ok = true;
    REP(i, n) {
      const P z = mov[worst_move[i]];
      const P &p1 = polygon[i] + z;
      const P &p2 = polygon[(i + 1) % n] + z;
      auto cp = cross(p2 - p1, qp - p1);
      if (cp != 0 and (cp >= 0) != inside[i]) {
        ok = false;
        break;
      }
    }
    print(ok);
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}
