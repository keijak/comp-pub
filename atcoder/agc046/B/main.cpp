//#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define debug(...)
#define DEBUG(...)
#else
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#endif

// auto mod int

const i64 MOD = 998244353;

struct mint {
  long long x;
  mint(long long x = 0) : x((x % MOD + MOD) % MOD) {}
  mint operator-() const { return mint(-x); }
  mint& operator+=(const mint a) {
    if ((x += a.x) >= MOD) x -= MOD;
    return *this;
  }
  mint& operator-=(const mint a) {
    if ((x += MOD - a.x) >= MOD) x -= MOD;
    return *this;
  }
  mint& operator*=(const mint a) {
    (x *= a.x) %= MOD;
    return *this;
  }
  mint operator+(const mint a) const { return mint(*this) += a; }
  mint operator-(const mint a) const { return mint(*this) -= a; }
  mint operator*(const mint a) const { return mint(*this) *= a; }
  mint pow(long long t) const {
    if (!t) return 1;
    mint a = pow(t >> 1);
    a *= a;
    if (t & 1) a *= *this;
    return a;
  }

  // for prime MOD
  mint inv() const { return pow(MOD - 2); }
  mint& operator/=(const mint a) { return *this *= a.inv(); }
  mint operator/(const mint a) const { return mint(*this) /= a; }
};
istream& operator>>(istream& is, mint& a) { return is >> a.x; }
ostream& operator<<(ostream& os, const mint& a) { return os << a.x; }

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int A, B, C, D;
  cin >> A >> B >> C >> D;
  vector<vector<mint>> dp(C + 1, vector<mint>(D + 1, 0));
  dp[A][B] = 1;
  for (int c = B + 1; c <= D; ++c) {
    dp[A][c] = dp[A][c - 1] * A;
  }
  for (int r = A + 1; r <= C; ++r) {
    dp[r][B] = dp[r - 1][B] * B;
  }
  REP(i, C - A) {
    int r = A + 1 + i;
    REP(j, D - B) {
      int c = B + 1 + j;
      dp[r][c] = (dp[r - 1][c] * c + dp[r][c - 1] * r -
                  dp[r - 1][c - 1] * (r - 1) * (c - 1));
    }
  }
  cout << dp[C][D] << endl;
}
