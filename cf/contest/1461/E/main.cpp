#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(y) std::begin(y), std::end(y)
using i64 = long long;
using u64 = unsigned long long;

template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &y : a) is >> y;
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

using Int = __int128_t;

bool solve() {
  i64 k, l, r, t, x, y;
  cin >> k >> l >> r >> t >> x >> y;
  Int z0 = k - l;
  Int b = r - l;
  Int w0 = z0;
  if (z0 + y <= r) {
    w0 = z0 + y;
  }
  Int z1 = w0 - x;
  if (z1 < 0) {
    return false;  // Game over on the first day
  }
  if (x > y) {
    // Gradually decreasing
    Int d = x - y;
    return z1 - d * (t - 1) >= 0;
  }

  // Simulation
  vector<bool> seen(x, false);
  for (Int z = z0; t > 0;) {
    Int wait = 0;
    if (z >= x) wait = z / x;
    z -= wait * x;
    assert(z >= 0);
    assert(z < x);
    t -= wait;
    if (t <= 0 or seen[z]) break;
    if (z + y > b) return false;
    seen[z] = true;
    z += y;
    z -= x;
    --t;
  }
  return true;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << (solve() ? "Yes" : "No") << "\n";
}
