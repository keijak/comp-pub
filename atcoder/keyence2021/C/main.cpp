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

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

i64 solve() {
  i64 H, W, K;
  cin >> H >> W >> K;
  vector grid(H, vector(W, '.'));
  REP(i, K) {
    int h, w;
    char c;
    cin >> h >> w >> c;
    --h, --w;
    grid[h][w] = c;
  }
  Mint all_count = Mint(3).pow(H * W - K);

  //   if (H < 10 and W < 10) {
  //     REP(i, H) {
  //       REP(j, W) { cerr << grid[i][j].c; }
  //       cerr << endl;
  //     }
  //   }

  auto co = [](char c) -> int {
    if (c == 'R') return 1;
    if (c == 'D') return 2;
    if (c == 'X') return 3;
    return 0;
  };

  vector dp(2, vector(W, Mint(0)));
  dp[1][0] = all_count;
  const Mint P = Mint(2) / 3;

  REP(i, H) {
    REP(j, W) {
      if (i == 0 and j == 0) continue;
      if (i > 0) {
        Mint r = 0;
        int xu = co(grid[i - 1][j]);
        if (xu == 0) {
          r = dp[0][j] * P;
        } else if (xu == 2 or xu == 3) {
          r = dp[0][j];
        }
        dp[1][j] += r;
      }
      if (j > 0) {
        Mint r = 0;
        int xl = co(grid[i][j - 1]);
        if (xl == 0) {
          r = dp[1][j - 1] * P;
        } else if (xl == 1 or xl == 3) {
          r = dp[1][j - 1];
        }
        dp[1][j] += r;
      }
    }
    swap(dp[0], dp[1]);
    fill(ALL(dp[1]), Mint(0));
  }
  Mint ans = dp[0][W - 1];
  return ans.val();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
