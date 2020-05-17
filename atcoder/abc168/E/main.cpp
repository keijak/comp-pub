#include <bits/stdc++.h>

#include <boost/rational.hpp>

#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
using boost::rational;

const i64 MOD = 1000000007;

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
  std::ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N;
  cin >> N;
  i64 cnt_ab0 = 0, cnt_a0 = 0, cnt_b0 = 0;
  map<rational<i64>, i64> cnt_r;

  REP(i, N) {
    i64 a, b;
    cin >> a >> b;
    if (a == 0 && b == 0) {
      ++cnt_ab0;
    } else if (a == 0) {
      ++cnt_a0;
    } else if (b == 0) {
      ++cnt_b0;
    } else {
      rational<i64> r{a, b};
      ++cnt_r[move(r)];
    }
  }

  set<rational<i64>> rpairs;
  for (auto&& [r, val] : cnt_r) {
    if (r > 0) {
      rpairs.insert(r);
    } else {
      rpairs.insert(-1 / r);
    }
  }

  vector<mint> pow2(N + 1);
  pow2[0] = 1;
  REP(i, N) { pow2[i + 1] = pow2[i] * 2; }

  mint ans{1};
  for (const auto& r1 : rpairs) {
    auto r2 = -1 / r1;
    ans *= pow2[cnt_r[r1]] + pow2[cnt_r[r2]] - 1;
  }
  ans *= pow2[cnt_a0] + pow2[cnt_b0] - 1;
  ans += cnt_ab0 - 1;
  cout << ans << endl;
}
