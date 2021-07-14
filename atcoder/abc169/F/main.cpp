#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

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
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, S;
  cin >> N >> S;
  vector<i64> A(N);
  for (auto& x : A) cin >> x;

  vector<vector<mint>> dp(N + 1, vector<mint>(S + 1, 0));
  dp[0][0] = 1;
  for (int i = 0; i < N; ++i) {
    i64 a = A[i];
    REP(j, S + 1) { dp[i + 1][j] = dp[i][j] * 2; }
    for (int j = S - a; j >= 0; --j) {
      dp[i + 1][j + a] += dp[i][j];
    }
  }
  cout << dp[N][S] << endl;
}
