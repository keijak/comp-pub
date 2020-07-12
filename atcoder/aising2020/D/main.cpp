#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

i64 MOD = 1'000'000'007;

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
  i64 N;
  cin >> N;
  string X;
  cin >> X;
  const int L = 2'000'005;
  vector<i64> memo(L, -1);
  memo[0] = 0;
  auto f = [&](auto rec, i64 x) -> int {
    assert(x < L);
    if (memo[x] != -1) {
      return memo[x];
    }
    i64 pc = __builtin_popcountll(x);
    return memo[x] = (rec(rec, x % pc) + 1);
  };

  int xpc = 0;
  REP(i, N) {
    if (X[i] == '1') ++xpc;
  }
  DEBUG(xpc);

  vector<i64> pows1(N);  // mod xpc-1
  vector<i64> pows2(N);  // mod xpc+1
  i64 p1 = 1, p2 = 1;
  i64 zp1 = 0, zp2 = 0;
  REP(i, N) {
    if (xpc > 1) {
      pows1[i] = p1 % (xpc - 1);
      p1 = (p1 * 2) % (xpc - 1);
      if (X[N - 1 - i] == '1') {
        zp1 += pows1[i];
        zp1 %= xpc - 1;
      }
      DEBUG(i, pows1[i], zp1);
    }
    pows2[i] = p2 % (xpc + 1);
    p2 = (p2 * 2) % (xpc + 1);
    if (X[N - 1 - i] == '1') {
      zp2 += pows2[i];
      zp2 %= xpc + 1;
    }
    // DEBUG(pows1[i], zp1, pows2[i], zp2);
  }
  DEBUG(zp1);

  REP(i, N) {
    if (X[i] == '1') {
      if (xpc == 1) {
        cout << 0 << '\n';
      } else {
        i64 mod = xpc - 1;
        i64 zp = (zp1 - pows1[N - 1 - i] + mod) % mod;
        // cout << "_" << '\n';
        cout << f(f, zp) + 1 << '\n';
      }
    } else {
      i64 mod = xpc + 1;
      i64 zp = (zp2 + pows2[N - 1 - i] + mod) % mod;
      cout << f(f, zp) + 1 << '\n';
    }
  }
}
