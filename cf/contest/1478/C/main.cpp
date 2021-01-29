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
using Int = __int128_t;

bool solve() {
  int n;
  cin >> n;
  const int n2 = n * 2;
  vector<i64> d(n2);
  cin >> d;
  sort(ALL(d), greater<>());
  REP(i, n) {
    if (d[2 * i] != d[2 * i + 1]) return false;
    if (i < n - 1 and d[2 * i] == d[2 * (i + 1)]) return false;
  }
  if (n == 1) {
    return d[0] > 0 and d[0] % 2 == 0;
  }

  DEBUG(d);
  vector<i64> z;
  z.reserve(n);
  Int zsum = 0;
  for (int i = 2; i < n2; i += 2) {
    int k = i - 2;  // prev
    i64 di = d[i];
    i64 dk = d[k];
    assert(di < dk);
    if ((dk - di) % (n2 - i) != 0) return false;
    i64 zi = (dk - di) / (n2 - i);
    z.push_back(zi);
    zsum += zi * i;
  }
  DEBUG(z);
  i64 dm = d.back();
  DEBUG(dm);
  if (zsum > dm) return false;
  Int zm = dm - zsum;
  if (zm == 0) return false;
  if (zm % n != 0) return false;
  zm /= n;
  DEBUG((i64)zm);
  return zm % 2 == 0;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    DEBUG("TEST", i);
    cout << (solve() ? "YES" : "NO") << "\n";
  }
}
