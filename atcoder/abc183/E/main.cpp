#include <bits/stdc++.h>

#include <atcoder/modint>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}
template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
using V = std::vector<T>;
template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
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
  return pprint(a, ", ", "", &(os << "{")) << "}";
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
void pdebug(const T &value, const Ts &... args) {
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
using mint = atcoder::modint1000000007;

struct pc {
  mint r, d, rd;
  mint x;
  pc() : r(0), d(0), rd(0), x(0) {}
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int H, W;
  cin >> H >> W;
  V<string> grid(H);
  REP(i, H) cin >> grid[i];
  //   V<V<pc>> prec(H, V<pc>(W));

  //   REP(i, H) {
  //     for (int j = W - 1; j >= 0; --j) {
  //       if (j == W - 1 or grid[i][j + 1] == '#' or grid[i][j] == '#') {
  //         prec[i][j].r = 0;
  //       } else {
  //         prec[i][j].r = prec[i][j + 1].r + 1;
  //       }
  //     }
  //   }

  //   REP(j, W) {
  //     for (int i = H - 1; i >= 0; --i) {
  //       if (i == H - 1 or grid[i + 1][j] == '#' or grid[i][j] == '#') {
  //         prec[i][j].d = 0;
  //       } else {
  //         prec[i][j].d = prec[i + 1][j].d + 1;
  //       }
  //     }
  //   }

  //   for (int i = H - 1; i >= 0; --i) {
  //     for (int j = W - 1; j >= 0; --j) {
  //       if (i == H - 1 or j == W - 1 or grid[i + 1][j + 1] == '#' or
  //           grid[i][j] == '#') {
  //         prec[i][j].rd = 0;
  //       } else {
  //         prec[i][j].rd = prec[i + 1][j + 1].rd + 1;
  //       }
  //     }
  //   }

  V<V<pc>> dp(H, V<pc>(W));
  dp[0][0].x = dp[0][0].r = dp[0][0].d = dp[0][0].rd = 1;
  REP(i, H) {
    REP(j, W) {
      if (i == 0 and j == 0) continue;
      if (grid[i][j] == '#') {
        dp[i][j].x = dp[i][j].r = dp[i][j].d = dp[i][j].rd = 0;
        continue;
      }
      dp[i][j].x = 0;
      if (j - 1 >= 0) {
        dp[i][j].x += dp[i][j - 1].r;
      }
      if (i - 1 >= 0) {
        dp[i][j].x += dp[i - 1][j].d;
      }
      if (i - 1 >= 0 and j - 1 >= 0) {
        dp[i][j].x += dp[i - 1][j - 1].rd;
      }

      dp[i][j].r = dp[i][j].d = dp[i][j].rd = dp[i][j].x;
      if (j - 1 >= 0) {
        dp[i][j].r += dp[i][j - 1].r;
      }
      if (i - 1 >= 0) {
        dp[i][j].d += dp[i - 1][j].d;
      }
      if (i - 1 >= 0 and j - 1 >= 0) {
        dp[i][j].rd += dp[i - 1][j - 1].rd;
      }
    }
  }
  cout << dp[H - 1][W - 1].x.val() << endl;

  //   REP(i, H) {
  //     REP(j, W) { cerr << ' ' << dp[i][j].x.val(); }
  //     cerr << endl;
  //   }
  //   cerr << endl;
  //   REP(i, H) {
  //     REP(j, W) { cerr << ' ' << dp[i][j].r.val(); }
  //     cerr << endl;
  //   }
  //   cerr << endl;
  //   REP(i, H) {
  //     REP(j, W) { cerr << ' ' << dp[i][j].d.val(); }
  //     cerr << endl;
  //   }
  //   cerr << endl;
  //   REP(i, H) {
  //     REP(j, W) { cerr << ' ' << dp[i][j].rd.val(); }
  //     cerr << endl;
  //   }
}
