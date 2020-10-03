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

template <unsigned int M>
struct ModInt {
  constexpr ModInt(long long val = 0) : _v(0) {
    if (val < 0) {
      long long k = (abs(val) + M - 1) / M;
      val += k * M;
    }
    assert(val >= 0);
    _v = val % M;
  }

  static constexpr int mod() { return M; }
  static constexpr unsigned int umod() { return M; }
  inline unsigned int val() const { return _v; }

  ModInt &operator++() {
    _v++;
    if (_v == umod()) _v = 0;
    return *this;
  }
  ModInt &operator--() {
    if (_v == 0) _v = umod();
    _v--;
    return *this;
  }
  ModInt operator++(int) {
    auto result = *this;
    ++*this;
    return result;
  }
  ModInt operator--(int) {
    auto result = *this;
    --*this;
    return result;
  }

  constexpr ModInt operator-() const { return ModInt(-_v); }
  constexpr ModInt &operator+=(const ModInt &a) {
    if ((_v += a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator-=(const ModInt &a) {
    if ((_v += M - a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator*=(const ModInt &a) {
    _v = ((unsigned long long)(_v)*a._v) % M;
    return *this;
  }
  constexpr ModInt pow(unsigned long long t) const {
    ModInt base = *this;
    ModInt res = 1;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }

  constexpr ModInt inv() const {
    // Inverse by Extended Euclidean algorithm.
    // M doesn't need to be prime, but x and M must be coprime.
    assert(_v != 0);
    // int g, x, y;
    // g = ext_gcd(_v, M, x, y);
    auto [g, x, y] = ext_gcd(_v, M);
    assert(g == 1LL);  // gcd(_v, M) must be 1.
    return x;

    // Inverse by Fermat's little theorem.
    // M must be prime, but it's usually faster.
    // return pow(M - 2);
  }
  constexpr ModInt &operator/=(const ModInt &a) { return *this *= a.inv(); }

  friend constexpr ModInt operator+(const ModInt &a, const ModInt &b) {
    return ModInt(a) += b;
  }
  friend constexpr ModInt operator-(const ModInt &a, const ModInt &b) {
    return ModInt(a) -= b;
  }
  friend constexpr ModInt operator*(const ModInt &a, const ModInt &b) {
    return ModInt(a) *= b;
  }
  friend constexpr ModInt operator/(const ModInt &a, const ModInt &b) {
    return ModInt(a) /= b;
  }
  friend constexpr bool operator==(const ModInt &a, const ModInt &b) {
    return a._v == b._v;
  }
  friend constexpr bool operator!=(const ModInt &a, const ModInt &b) {
    return a._v != b._v;
  }
  friend std::istream &operator>>(std::istream &is, ModInt &a) {
    return is >> a._v;
  }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &a) {
    return os << a._v;
  }

 private:
  // Extended Euclidean algorithm
  // Returns (gcd(a,b), x, y) where `a*x + b*y == gcd(a,b)`.
  static std::tuple<int, int, int> ext_gcd(int a, int b) {
    int ax = 1, ay = 0, bx = 0, by = 1;
    for (;;) {
      if (b == 0) break;
      auto d = std::div(a, b);
      a = b;
      b = d.rem;
      ax -= bx * d.quot;
      std::swap(ax, bx);
      ay -= by * d.quot;
      std::swap(ay, by);
    }
    return {a, ax, ay};
  }

  unsigned int _v;  // raw value
};
const unsigned int MOD = 1'000'000'007;
using Mint = ModInt<MOD>;

// Factorials over ModInts.
struct Factorials {
  // factorials and inverse factorials.
  vector<Mint> fact, ifact;

  // n: max cached value.
  Factorials(int n) : fact(n + 1), ifact(n + 1) {
    assert(n < (int)MOD);
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) ifact[i - 1] = ifact[i] * i;
  }

  // Combination (nCk)
  Mint C(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }

  // Permutation (nPk)
  Mint P(int n, int k) {
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
  vector<vector<Mint>> dp(N, vector<Mint>(2));
  REP(i, N) {
    auto [r, c] = walls[i];
    dp[i][1] += fs.C(r + c, r) * fs.C(H - 1 - r + W - 1 - c, H - 1 - r);
    auto it = cs.upper_bound(c);
    while (it != cs.begin()) {
      --it;
      int p = it->second;
      auto [pr, pc] = walls[p];
      Mint x = fs.C(r - pr + c - pc, r - pr) *
               fs.C(H - 1 - r + W - 1 - c, H - 1 - r) *
               fs.C(H - 1 - pr + W - 1 - pc, H - 1 - pr).inv();
      dp[i][0] += dp[p][1] * x;
      dp[i][1] += dp[p][0] * x;
    }
    cs.emplace(c, i);
  }
  Mint a0, a1;
  a0 = fs.C((H - 1) + (W - 1), H - 1);
  REP(i, N) {
    a0 += dp[i][0];
    a1 += dp[i][1];
  }
  cout << (a0 - a1) << endl;
}
