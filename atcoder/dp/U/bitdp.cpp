#include <cassert>
#include <iostream>
#include <vector>
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

using namespace std;
const i64 INF = 1e15;

// Most Significant Set Bit (Highest One Bit)
int mssb_pos(unsigned x) {
  static const int CLZ_WIDTH = __builtin_clz(1);
  assert(x != 0);
  return CLZ_WIDTH - __builtin_clz(x);
}
inline unsigned mssb(unsigned int x) { return 1U << mssb_pos(x); }

i64 solve() {
  int n;
  cin >> n;
  vector a(n, vector(n, 0LL));
  REP(i, n) REP(j, n) { cin >> a[i][j]; }

  vector<i64> dp(1 << n, -INF);
  dp[0] = 0;
  REP(bits, 1, 1 << n) {
    i64 single_score = 0;
    REP(i, n) {
      if (not(bits & (1 << i))) continue;
      REP(j, i) {
        if (not(bits & (1 << j))) continue;
        single_score += a[i][j];
      }
    }
    chmax(dp[bits], single_score);

    int high_bit = mssb(bits);
    for (int sub = bits;;) {
      sub = (sub - 1) & bits;
      if (not(sub & high_bit)) break;
      int other = bits - sub;
      if (dp[sub] == -INF or dp[other] == -INF) continue;
      chmax(dp[bits], dp[sub] + dp[other]);
    }
  }
  return dp[(1 << n) - 1];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
