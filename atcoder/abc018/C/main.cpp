#include <bits/stdc++.h>
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

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int R, C, K;
  cin >> R >> C >> K;
  V<string> grid(R);
  REP(i, R) { cin >> grid[i]; }

  auto cul = vector(R + 1, vector(C + 2, 0));
  auto cur = vector(R + 1, vector(C + 2, 0));
  {
    REP(r, R) REP(c, C) {
      int white = (grid[r][c] == 'o');
      cul[r + 1][c + 1] = cul[r][c] + white;
      cur[r + 1][c + 1] = cur[r][c + 2] + white;
    }
  }

  auto dp = vector(R + 1, vector(C + 1, 0));
  int fullc = 0;
  {
    for (int r = 0; r < R; ++r) {
      for (int c = 0; c < C; ++c) {
        if (abs(K - 1 - r) + abs(K - 1 - c) <= K - 1) {
          ++fullc;
          int white = (grid[r][c] == 'o');
          if (white) {
            ++dp[1][1];
          }
        }
      }
    }
  }
  int height = 2 * (K - 1) + 1;
  DEBUG(fullc, height);
  REP(r, R - height + 1) {
    int rc = r + K - 1;
    int rb = rc + K - 1;
    if (r > 0) {
      int c = 0;
      int cc = c + K - 1;
      int cr = cc + K - 1;
      int p = dp[r][c + 1];
      DEBUG(r, p);
      p += cur[rb + 1][cc + 1] - cur[rc][cr + 2];
      p += cul[rb][cc] - cul[rc][c];
      p -= cur[rc][c + 1] - cur[r - 1][cc + 2];
      DEBUG(r, cur[rc][c + 1], cur[r - 1][cc + 2]);
      p -= cul[rc][cr + 1] - cul[r][cc + 1];
      DEBUG(r, cul[rc][cr + 1], cul[r][cc + 1]);
      dp[r + 1][c + 1] = p;
    }
    for (int c = 1; c <= C - height; ++c) {
      int cc = c + K - 1;
      int cr = cc + K - 1;
      int p = dp[r + 1][c];
      p += cul[rc + 1][cr + 1] - cul[r][cc];
      p += cur[rb + 1][cc + 1] - cur[rc + 1][cr + 1];
      p -= cur[rc + 1][c] - cur[r][cc + 1];
      p -= cul[rb + 1][cc] - cul[rc + 1][c];
      dp[r + 1][c + 1] = p;
    }
  }

  int ans = 0;
  REP(i, R + 1) REP(j, C + 1) {
    if (dp[i][j] == fullc) {
      ++ans;
    }
  }
  cout << ans << endl;
}
