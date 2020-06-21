#define DEBUGGING  // Enables DEBUG macro.
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
  string S;
  cin >> S;
  int K;
  cin >> K;
  const int N = S.size();
  int zero_cnt = 0;
  REP(i, N) {
    if (S[i] == '0') zero_cnt++;
  }
  if (zero_cnt == 0) {
    cout << 1 << endl;
    return 0;
  }

  vector<int> zero_pos(zero_cnt);
  int k = 0;
  REP(i, N) {
    if (S[i] == '0') {
      zero_pos[k] = i;
      k++;
    }
  }

  vector<vector<mint>> dp(zero_cnt, vector<mint>(N));
  REP(k, K + 1) {
    int j = zero_pos.back() + k;
    if (j < N) {
      dp[zero_cnt - 1][j] = 1;
    }
  }

  for (int i = zero_cnt - 2; i >= 0; --i) {
    int p = zero_pos[i];
    REP(k, K + 1) {
      for (int j = p + k + 1; j < N; ++j) {
        dp[i][p + k] += dp[i + 1][j];
      }
    }
  }
  REP(i, zero_cnt) {
    REP(j, N) { cerr << dp[i][j] << ' '; }
    cerr << endl;
  }
  mint ans = 0;
  REP(i, N) { ans += dp[0][i]; }
  cout << ans << endl;
}
