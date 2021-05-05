
#pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <type_traits>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)

template <typename T, typename U>
inline bool chmin(T& a, U b) {
  return a > b and ((a = std::move(b)), true);
}

struct Input {
  inline operator int() { return read_int<int>(); }
  inline operator long long() { return read_int<long long>(); }
  inline operator unsigned() { return read_int<unsigned>(); }

 private:
  template <typename T>
  static T read_int() {
    T ret = 0, sgn = 1;
    int ch = getchar_unlocked();
    while (isspace(ch)) {
      ch = getchar_unlocked();
    }
    if constexpr (!std::is_unsigned<T>::value) {
      if (ch == '-') {
        sgn = -1;
        ch = getchar_unlocked();
      }
    }
    for (; isdigit(ch); ch = getchar_unlocked()) {
      ret = (ret * 10) + (ch - '0');
    }
    ungetc(ch, stdin);
    if constexpr (std::is_unsigned<T>::value) {
      return ret;
    } else {
      return ret * sgn;
    }
  }
} input;

using namespace std;

const int BIG = 1e8;
const int NMAX = 11;
int a[NMAX][NMAX];
char bad[NMAX][NMAX];
int dp[1 << NMAX][NMAX];

int solve() {
  const int n = input;
  REP(i, n) REP(j, n) a[i][j] = input;

  const int m = input;
  REP(i, m) {
    const int x = int(input) - 1;
    const int y = int(input) - 1;
    bad[x][y] = bad[y][x] = true;
  }

  REP(i, 1 << n) fill(dp[i], dp[i] + n, BIG);
  REP(i, n) { dp[1 << i][i] = a[i][0]; }
  REP(bits, 1, 1 << n) {
    const int pc = __builtin_popcount(bits);
    auto& dp_cur = dp[bits];
    REP(nxt, n) {
      if (bits >> nxt & 1) continue;
      auto& dp_nxt = dp[bits | (1 << nxt)][nxt];
      const int cost = a[nxt][pc];
      REP(cur, n) {
        if (bad[cur][nxt]) continue;
        chmin(dp_nxt, dp_cur[cur] + cost);
      }
    }
  }
  int ans = BIG;
  const int FULL = (1 << n) - 1;
  const auto& dp_full = dp[FULL];
  REP(i, n) { chmin(ans, dp_full[i]); }
  return (ans == BIG) ? -1 : ans;
}

int main() { printf("%d\n", solve()); }
