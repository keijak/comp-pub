#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), N_##i = (b); i < N_##i; ++i)
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

#ifdef ENABLE_DEBUG
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

using namespace std;
using Int = i64;

static std::tuple<Int, Int, Int> ext_gcd(Int a, Int b) {
  int ax = 1, ay = 0, bx = 0, by = 1;
  for (;;) {
    if (b == 0) break;
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

// Returns floor(x / y).
i64 floor_div(i64 x, i64 y) {
  int sign = (x < 0 ? -1 : 1) * (y < 0 ? -1 : 1);
  if (sign == 1) {
    return abs(x) / abs(y);
  } else {
    i64 d = abs(y);
    i64 q = (abs(x) + d - 1) / d;
    return -q;
  }
}

i64 floor_mod(i64 x, i64 y) {
  i64 q = floor_div(x, y);
  return x - q * y;
}

i64 solve() {
  i64 n, s, k;
  cin >> n >> s >> k;

  Int q = n - s;
  auto [g, x, y] = ext_gcd(n, k);
  if (q % g != 0) return -1;
  q /= g;
  n /= g;
  k /= g;
  return floor_mod(y * q, n);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) { cout << solve() << "\n"; }
}
