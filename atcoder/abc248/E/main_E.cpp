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

// compatible with std::complex
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

  bool operator==(const Point2d &other) {
    return x == other.x and y == other.y;
  }
  bool operator!=(const Point2d &other) { return x != other.x or y != other.y; }

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
using L = array<Int, 3>;

auto solve() {
  int n = in, K = in;
  vector<P> ps(n);
  REP(i, n) {
    Int x = in, y = in;
    ps[i] = P(x, y);
  }
  if (K == 1) {
    print("Infinity");
    return;
  }
  set<L> lines;

  REP(i, n) {
    REP(j, i + 1, n) {
      Int dx = ps[i].x - ps[j].x, dy = ps[i].y - ps[j].y;
      Int a = dy;
      Int b = -dx;
      Int c = dx * ps[i].y - dy * ps[i].x;
      Int g = std::gcd(std::gcd(abs(a), abs(b)), abs(c));
      a /= g;
      b /= g;
      c /= g;
      if (a < 0) {
        a *= -1;
        b *= -1;
        c *= -1;
      }
      if (a == 0 and b < 0) {
        b *= -1;
        c *= -1;
      }
      lines.insert({a, b, c});
    }
  }
  Int ans = 0;
  for (const auto &[a, b, c]: lines) {
    DUMP(a, b, c);
    int cnt = 0;
    REP(i, n) {
      if (a * ps[i].x + b * ps[i].y + c == 0) {
        ++cnt;
      }
    }
    if (cnt >= K) {
      ++ans;
    }
  }

  print(ans);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}
