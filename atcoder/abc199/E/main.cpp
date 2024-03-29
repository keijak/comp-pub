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

inline int popcount(unsigned x) { return __builtin_popcount(x); }

i64 solve() {
  int n, m;
  cin >> n >> m;

  vector<tuple<int, int, int>> constraint(m);
  REP(i, m) {
    int x, y, z;
    cin >> x >> y >> z;
    constraint[i] = {x, y, z};
  }
  sort(ALL(constraint));

  vector dp(1 << n, 0LL);
  dp[0] = 1;
  REP(bits, 1 << n) {
    int pc = popcount(bits);
    bool ok = true;
    for (int j = m - 1; j >= 0; --j) {
      const auto &[x, y, z] = constraint[j];
      if (x < pc) break;
      int cnt = 0;
      for (int q = 0; q < y; ++q) {
        if (bits & (1 << q)) ++cnt;
      }
      if (cnt > z) {
        ok = false;
        break;
      }
    }
    if (!ok) continue;

    REP(i, n) {
      if (bits & (1 << i)) {
        unsigned pbits = bits & ~(1 << i);
        dp[bits] += dp[pbits];
      }
    }
  }
  const int full = (1 << n) - 1;
  return dp[full];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
