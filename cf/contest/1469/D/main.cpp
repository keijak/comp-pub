#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(y) std::begin(y), std::end(y)
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
  for (auto &y : a) is >> y;
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

vector<pair<int, int>> solve() {
  i64 n;
  cin >> n;
  vector<int> a(n);
  REP(i, n) a[i] = i + 1;
  i64 y = n - 1;
  i64 x = n - 2;
  vector<pair<int, int>> ans;
  while (true) {
    i64 z = clamp(i64(sqrt(a[y]) + 0.5) - 1, 1LL, y - 1);
    while (x > z) {
      ans.emplace_back(x + 1, y + 1);
      a[x] = (a[x] + a[y] - 1) / a[y];
      x--;
    }
    ans.emplace_back(y + 1, z + 1);
    a[y] = (a[y] + a[z] - 1) / a[z];
    DEBUG(z, a[z], a);
    if (z == 1) {
      while (a[y] > 1) {
        ans.emplace_back(y + 1, z + 1);
        a[y] = (a[y] + a[z] - 1) / a[z];
      }
      DEBUG(a);
      break;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    DEBUG("case", i);
    auto res = solve();
    cout << ssize(res) << "\n";
    for (auto [y, x] : res) {
      cout << y << ' ' << x << "\n";
    }
  }
}
