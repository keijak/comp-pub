#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128_t;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}
template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
using V = std::vector<T>;
template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
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
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;

template <typename T>  // T: int, double, etc.
struct Point2d {
  T x;
  T y;

  Point2d() : x(0), y(0) {}
  Point2d(T x, T y) : x(x), y(y) {}
  Point2d(const Point2d &) = default;
  Point2d(Point2d &&) = default;
  Point2d &operator=(const Point2d &) = default;
  Point2d &operator=(Point2d &&) = default;

  // inner product
  T dot(const Point2d &other) const { return x * other.x + y * other.y; }

  // outer product
  T cross(const Point2d &other) const { return y * other.x - x * other.y; }

  T abs2() const { return this->dot(*this); }               // |x|^2
  double abs() const { return std::sqrt((double)abs2()); }  // |x|

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

using P = Point2d<i64>;

std::ostream &operator<<(std::ostream &os, const P &a) {
  return os << "(" << a.x << ", " << a.y << ")";
}

const double EPS = 1e-6;

// Convex Hull by Monotone Chain
// Populates the upper hull and the lower hull separately.
void scan_convex_hull(const vector<P> &ps, vector<P> &upper, vector<P> &lower) {
  for (int i = 0; i < ssize(ps); ++i) {
    auto ax = ps[i].x, ay = ps[i].y;
    P now{ax, ay};
    while (upper.size() >= 2) {
      P &p2 = upper[upper.size() - 2];
      P v1 = upper.back() - p2;
      P v2 = now - p2;
      if (v1.cross(v2) > 0) break;
      upper.pop_back();
    }
    upper.push_back(move(now));
  }
  for (int i = ps.size() - 1; i >= 0; --i) {
    auto ax = ps[i].x, ay = ps[i].y;
    P now{ax, ay};
    while (lower.size() >= 2) {
      P &p2 = lower[lower.size() - 2];
      P v1 = lower.back() - p2;
      P v2 = now - p2;
      if (v1.cross(v2) > 0) {
        break;
      }
      lower.pop_back();
    }
    lower.push_back(move(now));
  }
  reverse(lower.begin(), lower.end());
}
// relative positions of a->b and a->c.
int ccw(P a, P b, P c) {
  b -= a;
  c -= a;
  if (b.cross(c) > 0) return +1;       // counter clockwise
  if (b.cross(c) < 0) return -1;       // clockwise
  if (b.dot(c) < 0) return +2;         // c--a--b on line
  if (b.abs2() < c.abs2()) return -2;  // a--b--c on line or a==b
  return 0;                            // a--c--b on line or a==c or b==c
}

// intersection: line and point
bool isecLP(P a1, P a2, P b) {
  return abs(ccw(a1, a2, b)) != 1;  // return EQ(cross(a2-a1, b-a1), 0); と等価
}

// intersection: line and line
bool isecLL(P a1, P a2, P b1, P b2) {
  return !isecLP(a2 - a1, b2 - b1, {0, 0}) || isecLP(a1, b1, b2);
}
std::optional<P> crosspointLL(P a1, P a2, P b1, P b2) {
  i64 d1 = (b2 - b1).cross(b1 - a1);
  i64 d2 = (b2 - b1).cross(a2 - a1);
  if ((d1 == 0) && (d2 == 0)) return a1;  // same line
  if (d2 == 0) return std::nullopt;       // parallel
  return a1 + d1 / d2 * (a2 - a1);
}

void solve() {
  i64 N, S;
  cin >> N >> S;
  V<P> points(N);
  REP(i, N) {
    i64 x, y;
    cin >> x >> y;
    points[i] = {x, y};
  }
  V<P> upper, chull;
  scan_convex_hull(points, upper, chull);
  DEBUG(upper);
  for (int i = ssize(upper) - 2; i > 0; --i) {
    chull.push_back(move(upper[i]));
  }
  DEBUG(chull);
  int m = ssize(chull);
  REP(i, m) {
    const auto &p1 = chull[i];
    int j = (i - 1 + m) % m;
    int k = (i + 1) % m;

    const auto &p2 = chull[];
    const auto &p3 = chull[(i + 2) % m];
  }
  while (m > 3) {
    i64 mina = 1e18;
    int mini = -1;
    P minp;
    for (int i = 0; i < m; ++i) {
      const auto &p4 = chull[(i + 3) % m];
      auto cp = crosspointLL(p1, p2, p3, p4);
      P newp;
      if (not cp.has_value()) {
      } else {
        newp = cp.value();
      }
      i64 ar = abs((p2 - newp).cross(p3 - newp));
      if (chmin(mina, ar)) {
        mini = i;
        minp = newp;
      }
    }
    assert(mini >= 0);
    int j = (mini + 2) % m;
    if (j == 0) {
      chull[0] = minp;
      chull.pop_back();
    } else {
      chull[j - 1] = minp;
      chull.erase(chull.begin() + j);
    }
    --m;
  }
  assert(m == 3);
  DEBUG(chull);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  solve();
}
