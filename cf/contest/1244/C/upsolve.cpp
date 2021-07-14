#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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

#ifdef MY_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &... args) {
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

using Int = __int128_t;
Int Abs(Int x) { return x < 0 ? -x : x; }

// Returns floor(x / y).
Int floordiv(Int x, Int y) {
  assert(y != 0);
  if (x == 0) return 0;
  int sign = ((x < 0) ^ (y < 0)) ? -1 : 1;
  if (sign == 1) {
    return Abs(x) / Abs(y);
  } else {
    auto d = Abs(y);
    return -((Abs(x) + d - 1) / d);
  }
}

// Returns x mod y.
Int floormod(Int x, Int y) {
  auto q = floordiv(x, y);
  auto r = x - q * y;
  assert(r >= 0);
  return r;
}

// Etended Euclidean algorithm.
// Returns [g, x, y] where g = a*x + b*y = GCD(a, b).
// Either x or y is negative.
std::array<Int, 3> ext_gcd(Int a, Int b) {
  Int ax = 1, ay = 0, bx = 0, by = 1;
  while (b != 0) {
    auto r = a % b;
    auto q = a / b;
    a = b;
    b = r;
    ax -= bx * q;
    std::swap(ax, bx);
    ay -= by * q;
    std::swap(ay, by);
  }
  return {a, ax, ay};
}

using namespace std;

optional<array<i64, 3>> solve() {
  i64 n, p, w, d;
  cin >> n >> p >> w >> d;

  auto [g, x, y] = ext_gcd(w, d);
  if (p % g != 0) return std::nullopt;

  Int w2 = w / g, d2 = d / g, p2 = p / g;
  x *= p2;
  y *= p2;

  auto m = floordiv(x, d2);
  x -= m * d2;
  y += m * w2;
  assert(0 <= x and x < d2);
  if (y < 0) return nullopt;
  if (d2 - w2 >= 0) return nullopt;
  Int k = y / w2;
  x += k * d2;
  y -= k * w2;
  if (x + y > n) return nullopt;
  i64 z = n - x - y;
  assert(w * x + y * d == p);
  assert(x + y + z == n);
  assert(x >= 0);
  assert(y >= 0);
  assert(z >= 0);
  return std::array{i64(x), i64(y), z};
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  auto res = solve();
  if (!res) {
    cout << -1 << "\n";

  } else {
    auto a = res.value();
    cout << a[0] << ' ' << a[1] << ' ' << a[2] << "\n";
  }
}
