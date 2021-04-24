#include <algorithm>
#include <iostream>
#include <vector>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using u32 = unsigned;
using i64 = long long;
using u64 = unsigned long long;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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

Mint solve() {
  int H, W;
  cin >> H >> W;
  vector<u32> wall(H);
  REP(i, H) {
    string row;
    cin >> row;
    assert(ssize(row) == W);
    u32 w = 0;
    REP(j, W) {
      if (row[j] == '#') {
        w |= 1 << j;
      }
    }
    wall[i] = w;
  }

  vector<int> bmap(1 << W, -1);
  const u32 mask = (1 << W) - 1;
  vector<u32> row_bits;
  row_bits.reserve(150000);
  for (u32 bits = 0; bits <= mask; ++bits) {
    if (bits & ((bits << 1) | (bits >> 1))) continue;
    bmap[bits] = ssize(row_bits);
    row_bits.push_back(bits);
  }
  const int P = ssize(row_bits);
  vector<vector<int>> nexts(P);

  REP(j, P) {
    const u32 b1 = row_bits[j];
    auto dfs = [&](auto &dfs, int i, u32 b2) {
      if (i == W) {
        nexts[j].push_back(bmap[b2]);
        return;
      }
      dfs(dfs, i + 1, b2);
      if (b1 & (1 << i)) return;
      if (b1 & (1 << (i + 1))) return;
      if (i > 0 and (b1 & (1 << (i - 1)))) return;
      if (i > 0 and (b2 & (1 << (i - 1)))) return;
      dfs(dfs, i + 1, b2 | (1 << i));
    };
    dfs(dfs, 0, 0);
  }

  // cerr << "precalc done: " << P << endl;

  vector dp(2, vector(P, Mint(0)));
  for (int j = 0; j < P; ++j) {
    const u32 bits = row_bits[j];
    if (bits & wall[0]) continue;
    dp[0][j] = 1;
  }
  for (int i = 1; i < H; ++i) {
    for (int j = 0; j < P; ++j) {
      const u32 bits = row_bits[j];
      if (bits & wall[i]) continue;
      for (int p : nexts[j]) {
        dp[1][j] += dp[0][p];
      }
    }
    swap(dp[0], dp[1]);
    fill(ALL(dp[1]), 0);
  }

  Mint ans = 0;
  for (int j = 0; j < P; ++j) {
    ans += dp[0][j];
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
