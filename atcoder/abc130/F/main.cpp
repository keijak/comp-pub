#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <boost/rational.hpp>
using Rational = boost::rational<i64>;

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
i64 BIG = 1e9;

Rational solve() {
  int n;
  cin >> n;
  vector<Rational> xr, xl, xs, yr, yl, ys;
  REP(i, n) {
    i64 x, y;
    char d;
    cin >> x >> y >> d;
    if (d == 'R') {
      xr.push_back(x);
      ys.push_back(y);
    } else if (d == 'L') {
      xl.push_back(x);
      ys.push_back(y);
    } else if (d == 'U') {
      xs.push_back(x);
      yr.push_back(y);
    } else {
      xs.push_back(x);
      yl.push_back(y);
    }
  }
  sort(ALL(xr));
  sort(ALL(xl));
  sort(ALL(xs));
  sort(ALL(yr));
  sort(ALL(yl));
  sort(ALL(ys));

  auto f = [&](vector<Rational> &zr, vector<Rational> &zl,
               vector<Rational> &zs) -> vector<Rational> {
    vector<Rational> ts;
    if (!zr.empty() and !zs.empty()) {
      ts.push_back(zs.back() - zr.back());
      ts.push_back(zs.front() - zr.front());
    }
    if (!zl.empty() and !zs.empty()) {
      ts.push_back(zl.back() - zs.back());
      ts.push_back(zl.front() - zs.front());
    }
    if (!zl.empty() and !zr.empty()) {
      ts.push_back(Rational(zl.back() - zr.back()) / Rational(2));
      ts.push_back(Rational(zl.front() - zr.front()) / Rational(2));
    }
    return ts;
  };

  set<Rational> ts;
  auto tx = f(xr, xl, xs);
  auto ty = f(yr, yl, ys);
  ts.insert(0);
  for (auto t : tx) {
    if (t > 0) ts.insert(t);
  }
  for (auto t : ty) {
    if (t > 0) ts.insert(t);
  }

  auto area = [&](Rational t) -> Rational {
    Rational xmax = i64(-1e18);
    if (!xr.empty()) chmax(xmax, xr.back() + t);
    if (!xs.empty()) chmax(xmax, xs.back());
    if (!xl.empty()) chmax(xmax, xl.back() - t);
    Rational xmin = i64(1e18);
    if (!xr.empty()) chmin(xmin, xr.front() + t);
    if (!xs.empty()) chmin(xmin, xs.front());
    if (!xl.empty()) chmin(xmin, xl.front() - t);
    Rational ymax = i64(-1e18);
    if (!yr.empty()) chmax(ymax, yr.back() + t);
    if (!ys.empty()) chmax(ymax, ys.back());
    if (!yl.empty()) chmax(ymax, yl.back() - t);
    Rational ymin = i64(1e18);
    if (!yr.empty()) chmin(ymin, yr.front() + t);
    if (!ys.empty()) chmin(ymin, ys.front());
    if (!yl.empty()) chmin(ymin, yl.front() - t);
    return (xmax - xmin) * (ymax - ymin);
  };
  Rational ans = Rational(i64(1e18));
  for (auto t : ts) {
    auto ar = area(t);
    chmin(ans, ar);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  auto res = solve();
  cout << boost::rational_cast<long double>(res) << "\n";
}
