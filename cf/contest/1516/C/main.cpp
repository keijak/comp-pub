#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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

vector<int> solve() {
  int n;
  cin >> n;
  vector<i64> a(n);
  cin >> a;
  i64 g = a[0];
  REP(i, 1, n) { g = gcd(g, a[i]); }
  REP(i, n) { a[i] /= g; }

  i64 s = accumulate(ALL(a), 0LL);
  if (s & 1) return {};

  i64 half = s / 2;
  vector dp(n + 1, vector(half + 1, false));
  dp[0][0] = true;
  REP(i, n) {
    i64 w = a[i];
    for (int j = half - w; j >= 0; --j) {
      if (dp[i][j]) dp[i + 1][j + w] = true;
    }
    for (int j = half; j >= 0; --j) {
      if (dp[i][j]) dp[i + 1][j] = true;
    }
  }
  if (not dp[n][half]) return {};

  vector<int> ans;
  REP(i, n) {
    if (a[i] & 1) {
      ans.push_back(i + 1);
      return ans;
    }
  }
  assert(false);
  return {};
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto res = solve();
  cout << ssize(res) << "\n";
  if (not res.empty()) print_seq(res);
}
