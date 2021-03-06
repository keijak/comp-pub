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

bool solve() {
  i64 n, k;
  cin >> n >> k;
  vector<i64> h(n);
  cin >> h;
  //   vector<i64> lmax(n), rmax(n);
  //   lmax[0] = h[0];          // ground
  //   rmax[n - 1] = h[n - 1];  // ground
  //   REP(i, 1, n) {
  //     lmax[i] = lmax[i - 1] + k - 1;
  //     int ri = n - 1 - i;
  //     rmax[ri] = rmax[ri + 1] + k - 1;
  //   }
  //   REP(i, n) {
  //     if (min(lmax[i], rmax[i]) < h[i]) return false;
  //   }

  i64 lb = max(h[0] + 1 - k, h[1]);
  i64 ub = min(h[0] + k - 1, h[1] + k - 1);
  REP(i, 1, n - 1) {
    DEBUG(i, h[i], lb, ub);
    if (ub < h[i]) return false;
    if (lb > h[i] + k - 1) return false;
    lb = max(lb + 1 - k, h[i + 1]);
    ub = min(ub + k - 1, h[i + 1] + k - 1);
    if (ub < lb) return false;
  }
  return lb <= h[n - 1] and h[n - 1] <= ub;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    DEBUG(i);
    cout << (solve() ? "YES" : "NO") << "\n";
  }
}
