#include <bits/stdc++.h>

#include <boost/multiprecision/cpp_int.hpp>
#include <cmath>
// using Int = boost::multiprecision::cpp_int;
// using Rational = boost::multiprecision::cpp_rational;

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128_t;
using Int = long long;
using Float = long double;

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
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream *os = nullptr) {
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
  return print_seq(a, ", ", "", &(os << "{")) << "}";
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

Float EPS = 1e-9;

const Int BIG = Int(2e5) + 5;

// Returns ceil(x / y).
Int ceildiv(Int x, Int y) {
  if (y <= 0) {
    assert(y != 0);
    y *= -1;
    x *= -1;
  }
  if (x >= 0) {
    return (x + y - 1) / y;
  } else {
    return -((-x) / y);
  }
}

// Returns floor(x / y).
Int floordiv(Int x, Int y) {
  if (y <= 0) {
    assert(y != 0);
    y *= -1;
    x *= -1;
  }
  if (x >= 0) {
    return x / y;
  } else {
    return -((-x + y - 1) / y);
  }
}

Int floorsqrt(Int x) {
  assert(x >= 0);
  if (x <= 1) return x;
  Int r = sqrtl((long double)x);
  while (r * r < x) ++r;
  while (r * r > x) --r;
  return r;
}

Int ceilsqrt(Int x) {
  assert(x >= 0);
  if (x <= 1) return x;
  Int r = sqrtl((long double)x);
  while (r * r > x) --r;
  while (r * r < x) ++r;
  return r;
}

const Int D = 10000;

Int solve() {
  Float X, Y, R;
  cin >> X >> Y >> R;
  if (X < 0) X *= -1;
  if (Y < 0) Y *= -1;
  Int xp = Int(X * D + 0.1) % D;
  Int yp = Int(Y * D + 0.1) % D;
  Int rp = Int(R * D + 0.1);
  Int rp2 = rp * rp;
  DEBUG(xp, yp, rp);

  Int res = 0;
  Int start = ceildiv(xp - rp, D) * D;
  Int end = floordiv(xp + rp, D) * D;

  for (Int i = start; i <= end; i += D) {
    Int xd2 = (xp - i) * (xp - i);
    Int yd2 = rp2 - xd2;
    Int yd = ceilsqrt(yd2);
    Int ytop = ceildiv(yp + yd, D) * D;
    while (ytop > yp and (ytop - yp) * (ytop - yp) > yd2) ytop -= D;
    Int ybot = floordiv(yp - yd, D) * D;
    while (ybot < yp and (yp - ybot) * (yp - ybot) > yd2) ybot += D;
    if (ytop >= ybot) {
      res += (ytop - ybot) / D + 1;
    }
  }
  return res;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
