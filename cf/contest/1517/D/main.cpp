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

const array<int, 4> dx = {1, 0, -1, 0}, dy = {0, 1, 0, -1};

const u64 M1 = 500;
const u64 M2 = M1 * M1;
const u64 M3 = M1 * M1 * M1;

u64 encode(u64 x1, u64 y1, u64 x2, u64 y2) {
  if (x1 > x2) {
    swap(x1, x2);
    swap(y1, y2);
  } else if (x1 == x2 and y1 > y2) {
    swap(x1, x2);
    swap(y1, y2);
  }
  return x1 * M3 + y1 * M2 + x2 * M1 + y2;
}

vector<vector<i64>> solve() {
  int n, m, k;
  cin >> n >> m >> k;
  unordered_map<u64, i64> cost;
  cost.reserve(1 << 20);
  cost.max_load_factor(0.25);

  REP(i, n) {
    REP(j, m - 1) {
      i64 c;
      cin >> c;
      u64 e = encode(i, j, i, j + 1);
      cost[e] = c;
    }
  }
  REP(i, n - 1) {
    REP(j, m) {
      i64 c;
      cin >> c;
      u64 e = encode(i, j, i + 1, j);
      cost[e] = c;
    }
  }

  if (k & 1) {
    return vector(n, vector(m, -1LL));
  }

  const i64 BIG = 1e16;
  const int half = k / 2;
  vector dp(2, vector(n, vector(m, 0LL)));
  REP(i, half) {
    REP(i, n) REP(j, m) { dp[1][i][j] = BIG; }
    REP(i, n) {
      REP(j, m) {
        REP(d, 4) {
          int ni = i + dx[d];
          int nj = j + dy[d];
          if (ni < 0 or ni >= n or nj < 0 or nj >= m) continue;
          const u64 e = encode(i, j, ni, nj);
          auto it = cost.find(e);
          assert(it != cost.end());
          const i64 c = it->second;
          chmin(dp[1][i][j], dp[0][ni][nj] + c);
        }
      }
    }
    swap(dp[1], dp[0]);
  }

  REP(i, n) {
    REP(j, m) {
      if (dp[0][i][j] == BIG) {
        dp[0][i][j] = -1;
      } else {
        dp[0][i][j] *= 2;
      }
    }
  }

  return dp[0];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto ans = solve();
  for (const auto &row : ans) {
    print_seq(row);
  }
}
