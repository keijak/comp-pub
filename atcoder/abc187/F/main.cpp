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

int mssb_pos(unsigned x) {
  static const int CLZ_WIDTH = __builtin_clz(1);
  // assert(x != 0);
  return CLZ_WIDTH - __builtin_clz(x);
}
inline unsigned mssb(unsigned int x) { return 1U << mssb_pos(x); }

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector g(n, vector(n, false));

  REP(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a][b] = g[b][a] = true;
  }

  vector<bool> is_clique(1 << n);
  REP(bits, 1 << n) {
    if (bits == 0) {
      is_clique[bits] = true;
      continue;
    }
    int r = mssb_pos(bits);
    unsigned r_mask = 1 << r;
    if (not is_clique[bits - r_mask]) continue;
    bool ok = true;
    REP(i, n) {
      if (not(bits & (1 << i))) continue;
      if (i != r and not g[i][r]) {
        ok = false;
        break;
      }
    }
    is_clique[bits] = ok;
  }

  vector<int> dp(1 << n, n);
  dp[0] = 0;
  REP(bits, 1, 1 << n) {
    dp[bits] = __builtin_popcount(bits);
    if (is_clique[bits]) {
      chmin(dp[bits], 1);
      continue;
    }
    int h_bit = mssb(bits);
    for (int sub = (bits - 1) & bits; sub & h_bit; sub = (sub - 1) & bits) {
      chmin(dp[bits], dp[sub] + dp[bits - sub]);
    }
  }
  return dp[(1 << n) - 1];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
