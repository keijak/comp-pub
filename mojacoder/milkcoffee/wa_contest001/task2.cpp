#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename T>
std::ostream &operator<<(std::ostream &os, const std::optional<T> &a) {
  if (a) {
    return os << "[" << *a << "]";
  } else {
    return os << "[]";
  }
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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

bool sol0(int H, int W) {
  // 0: no paint, 1: R, 2: G, 3: B
  auto grid = vector(H, vector(W, 0));
  array<int, 4> dr = {0, 1, 0, -1};
  array<int, 4> dc = {1, 0, -1, 0};

  auto dfs = [&](auto &dfs, int r, int c, int i) -> bool {
    if (r == H - 1 and c == W - 1) {
      return grid[r][c] == 3;
    }
    REP(d, 4) {
      int nr = r + dr[d];
      int nc = c + dc[d];
      if (nr < 0 or nr >= H or nc < 0 or nc >= W) continue;
      if (grid[nr][nc] != 0) continue;
      grid[nr][nc] = 1 + (i % 3);
      if (dfs(dfs, nr, nc, i + 1)) return true;
      grid[nr][nc] = 0;
    }
    return false;
  };
  return dfs(dfs, 0, 0, 0);
}

bool sol1(int H) { return (H - 1) % 3 == 0; }

bool sol2(int H) {
  auto dp = vector(H, vector(2, vector(2, vector(3, optional<int>()))));
  REP(vert, 2) REP(color, 3) dp[0][0][1][2] = 0;
  dp[0][0][1][2] = 1;

  auto f = [&](auto &f, int x, int y, bool vert, int color) -> bool {
    if (dp[x][y][vert][color].has_value()) {
      return dp[x][y][vert][color].value();
    }
    bool ok = false;
    if (x >= 1) {
      if (f(f, x - 1, y, true, (color - 1 + 3) % 3)) ok = true;
      if (f(f, x - 1, y, false, (color - 1 + 3) % 3)) ok = true;
    }
    if (not vert) {
      if (f(f, x, 1 - y, true, (color - 1 + 3) % 3)) ok = true;
    }
    dp[x][y][vert][color] = ok;
    return ok;
  };
  bool ok = f(f, H - 1, 1, false, 2) or f(f, H - 1, 1, true, 2);

  //   REP(color, 3) {
  //     cerr << "### COLOR = " << color << endl;
  //     for (int y = 0; y < 2; ++y) {
  //       for (int x = 0; x < H; ++x) {
  //         cerr << " {" << dp[x][y][false][2] << "; " << dp[x][y][true][2] <<
  //         "}";
  //       }
  //       cerr << endl;
  //     }
  //     cerr << endl;
  //   }
  return ok;
}

bool solve() {
  int H, W;
  cin >> H >> W;
  if (H < W) swap(H, W);
  if (H * W <= 8) {
    return sol0(H, W);
  }
  if (W == 1) {
    return sol1(H);
  }
  if (W == 2) {
    return sol2(H);
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << (solve() ? "Yes" : "No") << "\n";
}
