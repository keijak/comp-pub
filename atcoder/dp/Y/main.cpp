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

const int MOD = 1'000'000'007;

struct mint {
  int x;
  mint(long long val = 0) {
    if (val < 0) {
      long long k = (abs(val) + MOD - 1) / MOD;
      val += k * MOD;
    }
    x = val % MOD;
  }
  mint operator-() const { return mint(-x); }
  mint &operator+=(const mint &a) {
    if ((x += a.x) >= MOD) x -= MOD;
    return *this;
  }
  mint &operator-=(const mint &a) {
    if ((x += MOD - a.x) >= MOD) x -= MOD;
    return *this;
  }
  mint &operator*=(const mint &a) {
    x = (x * (long long)a.x) % MOD;
    return *this;
  }
  mint pow(long long t) const {
    assert(t >= 0);
    mint base = *this;
    mint res = 1;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }
  // for prime MOD
  mint inv() const { return pow(MOD - 2); }
  mint &operator/=(const mint &a) { return *this *= a.inv(); }
};
mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
bool operator==(const mint &a, const mint &b) { return a.x == b.x; }
bool operator!=(const mint &a, const mint &b) { return a.x != b.x; }
bool operator<(const mint &a, const mint &b) { return a.x < b.x; }
bool operator>(const mint &a, const mint &b) { return a.x > b.x; }
bool operator<=(const mint &a, const mint &b) { return a.x <= b.x; }
bool operator>=(const mint &a, const mint &b) { return a.x >= b.x; }
istream &operator>>(istream &is, mint &a) { return is >> a.x; }
ostream &operator<<(ostream &os, const mint &a) { return os << a.x; }

// Factorials over ModInts.
struct Factorials {
  // factorials and inverse factorials.
  vector<mint> fact, ifact;

  // n: max cached value.
  Factorials(int n) : fact(n + 1), ifact(n + 1) {
    assert(n < MOD);
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) ifact[i - 1] = ifact[i] * i;
  }

  // Combination (nCk)
  mint C(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }

  // Permutation (nPk)
  mint P(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[n - k];
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int H, W, N;
  cin >> H >> W >> N;
  vector<pair<int, int>> walls(N);
  REP(i, N) {
    int r, c;
    cin >> r >> c;
    r--;
    c--;
    walls[i] = {r, c};
  }
  sort(walls.begin(), walls.end());
  Factorials fs(H + W - 2);
  multimap<int, int> cs;
  vector<vector<mint>> dp(N, vector<mint>(2));
  REP(i, N) {
    auto [r, c] = walls[i];
    dp[i][1] += fs.C(r + c, r) * fs.C(H - 1 - r + W - 1 - c, H - 1 - r);
    auto it = cs.upper_bound(c);
    while (it != cs.begin()) {
      --it;
      int p = it->second;
      auto [pr, pc] = walls[p];
      mint x = fs.C(r - pr + c - pc, r - pr) *
               fs.C(H - 1 - r + W - 1 - c, H - 1 - r) *
               fs.C(H - 1 - pr + W - 1 - pc, H - 1 - pr).inv();
      dp[i][0] += dp[p][1] * x;
      dp[i][1] += dp[p][0] * x;
    }
    cs.emplace(c, i);
  }
  mint a0, a1;
  a0 = fs.C((H - 1) + (W - 1), H - 1);
  REP(i, N) {
    a0 += dp[i][0];
    a1 += dp[i][1];
  }
  cout << (a0 - a1) << endl;
}
