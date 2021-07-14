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

#include <atcoder/convolution>
#include <atcoder/modint>

using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

i64 solve() {
  i64 n, k;
  cin >> n >> k;
  vector<i64> a(n);
  cin >> a;
  vector<int> halve_count(n, 0);
  i64 halve_total = 0;
  REP(i, n) {
    i64 x = a[i];
    int cnt = 0;
    while (x) {
      x >>= 1;
      ++cnt;
    }
    halve_count[i] = cnt;
    halve_total += cnt;
  }
  chmin(k, halve_total);

  // dp[x][y][z]
  //   processed x items,
  //   y=0: includes zeros, y=1 no zeros
  //   z operations remaining
  vector dp(2, vector(2, vector(k + 1, Mint(0))));
  dp[0][1][k] = 1;
  REP(i, n) {
    int h = halve_count[i];
    REP(j, k + 1) {
      REP(p, h + 1) {
        if (j - p < 0) break;
        dp[1][0][j - p] += dp[0][0][j];
        if (p == h) dp[1][0][j - p] += dp[0][1][j];
        if (p < h) dp[1][1][j - p] += dp[0][1][j];
      }
    }
    swap(dp[0], dp[1]);
    dp[1][0].assign(k + 1, Mint(0));
    dp[1][1].assign(k + 1, Mint(0));
  }

  Mint ans = dp[0][1][0];
  REP(j, k + 1) { ans += dp[0][0][j]; }
  return ans.val();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
