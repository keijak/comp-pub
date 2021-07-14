#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <boost/multiprecision/cpp_int.hpp>
// using i128 = boost::multiprecision::checked_int128_t;
// using u128 = boost::multiprecision::checked_uint128_t;
using Int = boost::multiprecision::checked_cpp_int;
// using Rational = boost::multiprecision::checked_cpp_rational;
namespace multip = boost::multiprecision;

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

// Returns floor(x / y).
template <class T>
T floor_div(T x, T y) {
  assert(y != 0);
  if (y < 0) x = -x, y = -y;
  return (x >= 0) ? (x / y) : ((x - y + 1) / y);
}

// Etended Euclidean algorithm.
// Returns [g, x, y] where g = a*x + b*y = GCD(a, b).
// Note that g, x, y can be negative.
std::array<i64, 3> ext_gcd(i64 a, i64 b) {
  if (b == 0) return {a, 1LL, 0LL};
  auto res = ext_gcd(b, a % b);  // = (g, x, y)
  res[1] -= (a / b) * res[2];
  std::swap(res[1], res[2]);
  return res;  // = (g, y, x - (a/b)*y)
}

// Linear Diophantine equation (Bezout equation).
// Solves a*x + b*y = c (a,b,c: positive integers).
// Returns [x, y] where 0 <= x < b/gcd(a,b).
std::optional<std::array<i64, 2>> linear_diophantine(i64 a, i64 b, i64 c) {
  auto [g, x, y] = ext_gcd(a, b);
  if (g < 0) {
    g *= -1, x *= -1, y *= -1;
  }
  if (c % g != 0) return std::nullopt;
  a /= g, b /= g, c /= g;
  x *= c, y *= c;
  auto m = floor_div(x, b);
  x -= m * b;
  y += m * a;
  assert(0 <= x and x < b);
  return std::array{x, y};
}

vector<i64> solve() {
  i64 N;
  cin >> N;

  if (N <= 3) {
    return vector<i64>(N, 1LL);
  }

  vector<i64> ans = {1, 2};
  i64 x = 1, y = 1;

  vector<i64> fibs = {1, 1};
  for (unsigned i = 2;; ++i) {
    Int x = Int(fibs[i - 1]) + fibs[i - 2];
    if (x > N) break;
    fibs.push_back((i64)x);
  }
  const int m = ssize(fibs);

  {
    i64 op = (m % 2 == 1) ? 3 : 4;
    for (int i = 2; i < m; ++i) {
      ans.push_back(op);
      if (op == 3) {
        x += y;
      } else {
        y += x;
      }
      DUMP(op, x, y);
      op = (op == 3) ? 4 : 3;
    }
  }
  assert(x <= N);
  if (N == x) {
    return ans;
  }
  i64 rest = N - x;
  while (rest > 0) {
    auto it = upper_bound(ALL(fibs), rest);
    assert(it != fibs.begin());
    --it;
    i64 x = *it;
    int j = it - fibs.begin();
    auto pos = ans.end() - j;
    ans.insert(pos, j % 2 == 1 ? 2 : 1);
    rest -= x;
    // DUMP(j, x, ans, rest);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto ans = solve();
  cout << ans.size() << "\n";
  i64 x = 0, y = 0;
  for (auto op : ans) {
    cout << op << "\n";
    if (op == 1) {
      x += 1;
    } else if (op == 2) {
      y += 1;
    } else if (op == 3) {
      x += y;
    } else {
      y += x;
    }
    DUMP(op, x, y);
  }
}
