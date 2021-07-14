#include <bits/stdc++.h>

#include <atcoder/math>
#include <tuple>

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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;
using D = long double;      // Coordinate value
using P = std::complex<D>;  // Point

const D EPS = 1e-8;

// outer product: cross(a,b) = |a||b|sinθ
// for checking if two vectors are parallel.
D cross(const P &a, const P &b) { return (std::conj(a) * b).imag(); }

// Convex Hull by Monotone Chain
// Populates the upper hull and the lower hull separately.
void scan_convex_hull(const vector<P> &ps, vector<P> &upper, vector<P> &lower) {
  for (int i = 0; i < (int)ps.size(); ++i) {
    auto ax = ps[i].real(), ay = ps[i].imag();
    P now{ax, ay};
    while (upper.size() >= 2) {
      P &p2 = upper[upper.size() - 2];
      P v1 = upper.back() - p2;
      P v2 = now - p2;
      if (cross(v1, v2) > EPS) {
        break;
      }
      upper.pop_back();
    }
    upper.push_back(move(now));
  }
  for (int i = ps.size() - 1; i >= 0; --i) {
    auto ax = ps[i].real(), ay = ps[i].imag();
    P now{ax, ay};
    while (lower.size() >= 2) {
      P &p2 = lower[lower.size() - 2];
      P v1 = lower.back() - p2;
      P v2 = now - p2;
      if (cross(v1, v2) > EPS) {
        break;
      }
      lower.pop_back();
    }
    lower.push_back(move(now));
  }
  reverse(lower.begin(), lower.end());
}

i64 points_below(i64 x1, i64 y1, i64 x2, i64 y2) {
  // y-y1 = (y2-y1)/(x2-x1)(x - x1)
  // y = (y2-y1)/(x2-x1) * x + (x2-x1)/(x2-x1) * (y1-x1)
  i64 m = x2 - x1;
  assert(m > 0);
  i64 a = abs(y2 - y1);
  i64 g = gcd(a, m);
  i64 s1 = atcoder::floor_sum(m + 1, m / g, a / g, 0);
  s1 += (m + 1) * min(y1, y2);
  s1 -= y2;
  return s1;

  // const i64 m = x2 - x1;
  // const i64 a = y2 - y1;
  // const i64 b = (x2 - x1) * y1 - (y2 - y1) * x1;
  // const i64 s2 = atcoder::floor_sum(x2, m, a, b);
  // const i64 s1 = atcoder::floor_sum(x1, m, a, b);
  // return s2 - s1;
}

i64 integral(const vector<P> &hull, const set<pair<i64, i64>> &pset) {
  i64 res = 0;
  for (int i = 1; i < ssize(hull); ++i) {
    const P &p1 = hull[i - 1];
    const P &p2 = hull[i];
    const i64 x1 = llround(p1.real());
    const i64 y1 = llround(p1.imag());
    const i64 x2 = llround(p2.real());
    const i64 y2 = llround(p2.imag());
    assert(pset.count(pair(x2, y2)));
    if (x1 == x2) {
      if (y1 > y2) res += y1 - y2;
    } else {
      assert(x1 < x2);
      res += points_below(x1, y1, x2, y2);
    }
  }
  const P &plast = hull.back();
  res += plast.imag();
  return res;
}

auto solve() {
  int n;
  cin >> n;
  vector<P> points(n), upper, lower;
  set<pair<i64, i64>> pset;
  REP(i, n) {
    i64 x, y;
    cin >> x >> y;
    points[i] = {(D)x, (D)y};
    pset.insert(pair{x, y});
  }
  sort(ALL(points), [&](const P &p1, const P &p2) {
    return std::make_pair(p1.real(), p1.imag()) <
           std::make_pair(p2.real(), p2.imag());
  });
  scan_convex_hull(points, lower, upper);
  // DUMP(upper);
  // DUMP(lower);
  assert(upper.front() == lower.front());
  assert(upper.back() == lower.back());

  i64 ans = 0;
  const i64 s1 = integral(upper, pset);
  const i64 s2 = integral(lower, pset);
  ans += s1;
  ans -= s2;
  ans += ssize(lower);
  DUMP(s1, s2, ssize(lower));
  i64 lp = 0;
  for (int i = 1; i < ssize(lower); ++i) {
    const P &p1 = lower[i - 1];
    const P &p2 = lower[i];
    const i64 x1 = llround(p1.real());
    const i64 y1 = llround(p1.imag());
    const i64 x2 = llround(p2.real());
    const i64 y2 = llround(p2.imag());
    if (x1 == x2) {
      lp += abs(y1 - y2) - 1;
    } else {
      assert(x1 < x2);
      const i64 a = abs(x2 - x1);
      const i64 b = abs(y2 - y1);
      const i64 g = gcd(a, b);
      lp += g - 1;
    }
  }
  ans += lp;
  ans -= ssize(points);
  DUMP(lp, ssize(points));
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
