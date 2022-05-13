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

auto solve() {
  int n = in;
  vector<P> ps;
  ps.reserve(n);
  REP(i, n) {
    Int x = in, y = in;
    ps.emplace_back(x, y);
  }

  Int total_area2 = 0;  // area * 2
  const P &p0 = ps[0];
  REP(i, 2, n) {
    const P &p1 = ps[i - 1];
    const P &p2 = ps[i];
    total_area2 += abs(cross(p1 - p0, p2 - p0));
  }
  DUMP(total_area2);
//  Real target_area = total_area2 / 8.0;
//  DUMP(target_area);

  int r = 2;
  Int cur_area2 = 0;
  Int ans = kBigVal<Int>;
  for (int l = 0; l < n; ++l) {
    const P &pl = ps[l];
    while (cur_area2 * 4 < total_area2) {
      if (cur_area2 > 0) {
        if (cur_area2 > 0) {
          chmin(ans, abs(cur_area2 * 4 - total_area2));
        }
        Int s = total_area2 - cur_area2;
        if (s > 0) {
          chmin(ans, abs(total_area2 - s * 4));
        }
      }
      if (r > n * 2) break;
      const P &p1 = ps[(r - 1) % n];
      const P &p2 = ps[(r % n)];
      Int area2 = abs(cross(p1 - pl, p2 - pl));
      DUMP(l, r, area2);
      cur_area2 += area2;
      ++r;
    }
    {
      if (cur_area2 > 0) {
        chmin(ans, abs(cur_area2 * 4 - total_area2));
      }
      Int s = total_area2 - cur_area2;
      if (s > 0) {
        chmin(ans, abs(total_area2 - s * 4));
      }
    }
    if (l < n - 1) {
      const P &pr = ps[(r - 1) % n];
      const P &p1 = ps[l];
      const P &p2 = ps[l + 1];
      Int area2 = abs(cross(p1 - pr, p2 - pr));
      DUMP(l, r, area2);
      cur_area2 -= area2;
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
