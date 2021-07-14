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

// Most Significant Set Bit (Highest One Bit)
int mssb_pos(unsigned x) {
  static const int CLZ_WIDTH = __builtin_clz(1);
  return CLZ_WIDTH - __builtin_clz(x);
}
inline unsigned mssb(unsigned int x) { return 1U << mssb_pos(x); }

int main() {
  int N;
  scanf("%d", &N);
  vector<tuple<int, int>> times(N);
  int DAYM = 24 * 60;
  REP(i, N) {
    int h1, m1, h2, m2;
    scanf("%d:%d %d:%d", &h1, &m1, &h2, &m2);
    int t1 = (h1 * 60 + m1) % DAYM;
    int t2 = (h2 * 60 + m2) % DAYM;
    times[i] = {t2, t1};
  }

  vector g(N, vector(N, false));
  REP(i, N) {
    auto [e1, s1] = times[i];
    REP(j, N) {
      if (i == j) continue;
      auto [e2, s2] = times[j];
      bool overlap = true;
      if (s1 < e1 and s2 < e2) {
        if (e1 <= s2 or e2 <= s1) overlap = false;
      } else if (s1 < e1 and s2 > e2) {
        if (e1 <= s2 and e2 <= s1) overlap = false;
      } else if (s1 > e1 and s2 < e2) {
        if (e2 <= s1 and e1 <= s2) overlap = false;
      }
      g[i][j] = overlap;
    }
  }

  vector dp(1 << N, N);
  dp[0] = 0;
  for (unsigned bits = 1; bits < (1U << N); ++bits) {
    int m = __builtin_popcount(bits);
    chmin(dp[bits], m);

    bool ok = true;
    REP(i, N) {
      if (not(bits & (1 << i))) continue;
      REP(j, i) {
        if (not(bits & (1 << j))) continue;
        if (g[i][j]) {
          ok = false;
          break;
        }
      }
    }

    if (ok) {
      chmin(dp[bits], 1);
      continue;
    }

    unsigned h_bit = mssb(bits);
    for (unsigned sub = (bits - 1) & bits; sub & h_bit;
         sub = (sub - 1) & bits) {
      unsigned other = bits - sub;
      chmin(dp[bits], dp[sub] + dp[other]);
    }
  }
  printf("%d\n", dp[(1 << N) - 1]);
}
