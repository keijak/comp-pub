#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

const i64 MOD = 1'000'000'007;

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

i64 N, K;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cin >> N >> K;

  mint ans;
  ans += 1;            // no zero
  ans += N * (N - 1);  // 1 zero

  int L = min(N, K + 1);
  mint c1 = N, c2 = N - 1;
  // nCi * (n-1)Ci
  // i is the number of zeros.
  for (int i = 2; i < L; ++i) {
    mint mi = mint(i).inv();
    c1 *= mi * (N + 1 - i);
    c2 *= mi * (N - i);
    ans += c1 * c2;
  }
  cout << ans << endl;
}