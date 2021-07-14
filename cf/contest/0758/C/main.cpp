#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
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

#ifdef MY_DEBUG
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

array<i64, 3> solve() {
  i64 n, m, x, y, k;
  cin >> n >> m >> k >> x >> y;
  --x, --y;

  if (n == 1) {
    assert(x == 0);
    i64 p = k / m;
    i64 r = k % m;
    i64 mx = p + (r > 0 ? 1 : 0);
    i64 mn = p;
    i64 sg = p + (r > y ? 1 : 0);
    return {mx, mn, sg};
  }
  if (n == 2) {
    i64 p = k / (2 * m);
    i64 r = k % (2 * m);
    i64 mx = p + (r > 0 ? 1 : 0);
    i64 mn = p;
    i64 sg = p + (r > x * m + y ? 1 : 0);
    return {mx, mn, sg};
  }

  i64 p = k / (m * (2 * n - 2));
  i64 r = k % (m * (2 * n - 2));
  i64 mx = p * 2;
  if (r >= m + 1) {
    mx++;
  }
  if (r >= n * m + 1) {
    mx++;
  }
  i64 mn = p;
  if (r >= n * m) {
    mn++;
  }
  i64 sg = (x == 0 or x == n - 1) ? p : 2 * p;
  if (r >= x * m + y + 1) {
    ++sg;
  }
  if (x != 0 and x != n - 1 and r >= n * m + (n - 2 - x) * m + y + 1) {
    ++sg;
  }

  if (k <= m) {
    mx = 1;
    mn = 0;
  }

  return {mx, mn, sg};
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  auto a = solve();
  cout << a[0] << ' ' << a[1] << ' ' << a[2] << "\n";
}
