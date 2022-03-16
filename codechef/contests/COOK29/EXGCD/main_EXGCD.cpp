#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<unsigned M>
struct ModInt {
  ModInt() : _v(0) {}
  ModInt(long long val) {
    if (val < 0) {
      long long k = (std::abs(val) + M - 1) / M;
      val += k * M;
    }
    assert(val >= 0);
    _v = val % M;
  }

  static int mod() { return M; }
  static unsigned umod() { return M; }
  inline unsigned val() const { return _v; }

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

  ModInt operator-() const { return ModInt(umod() - _v); }

  ModInt &operator+=(const ModInt &a) {
    if ((_v += a._v) >= M) _v -= M;
    return *this;
  }
  ModInt &operator-=(const ModInt &a) {
    if ((_v += M - a._v) >= M) _v -= M;
    return *this;
  }
  ModInt &operator*=(const ModInt &a) {
    _v = ((unsigned long long) (_v) * a._v) % M;
    return *this;
  }
  ModInt pow(long long t) const {
    if (_v == 0) {
      return 0;  // corner case: 0^0 = ?
    }
    if (t < 0) {
      return this->inv().pow(-t);
    }
    ModInt base = *this;
    ModInt res = 1;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }

  // https://qiita.com/Mitarushi/items/8d7fb52e8a80e8008463
  ModInt inv() const {
    long long b = 1, a = _v;
    while (a > 1) {
      long long q = M / a;
      a = M - a * q;
      b = -b * q % M;
    }
    assert(a == 1);  // if a = 0, _v and M are not coprime.
    if (b < 0) b += M;
    ModInt ret;
    ret._v = (unsigned) b;
    return ret;
  }
  ModInt &operator/=(const ModInt &a) { return *this *= a.inv(); }

  friend ModInt operator+(const ModInt &a, const ModInt &b) {
    return ModInt(a) += b;
  }
  friend ModInt operator-(const ModInt &a, const ModInt &b) {
    return ModInt(a) -= b;
  }
  friend ModInt operator*(const ModInt &a, const ModInt &b) {
    return ModInt(a) *= b;
  }
  friend ModInt operator/(const ModInt &a, const ModInt &b) {
    return ModInt(a) /= b;
  }
  friend bool operator==(const ModInt &a, const ModInt &b) {
    return a._v == b._v;
  }
  friend bool operator!=(const ModInt &a, const ModInt &b) {
    return a._v != b._v;
  }
  friend std::istream &operator>>(std::istream &is, ModInt &a) {
    return is >> a._v;
  }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &a) {
    return os << a._v;
  }

 private:
  unsigned _v;  // raw value
};
const unsigned MOD = int(1e9) + 7;
using Mint = ModInt<MOD>;

template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;
//
//#ifdef MY_DEBUG
//#include "debug_dump.hpp"
//#include "backward.hpp"
//backward::SignalHandling kSignalHandling;
//#else
//#define DUMP(...)
//#define cerr if(false)cerr
//#endif

using namespace std;

struct LinearSieve {
  std::vector<int> spf;  // smallest prime factors.
  std::vector<int> mu;   // Moebius function
  std::vector<int> phi;  // Euler's totient function
  std::vector<int> primes;

  explicit LinearSieve(int n) : spf(n + 1), mu(n + 1), phi(n + 1) {
    mu[1] = 1;
    phi[1] = 1;
    for (int i = 2; i <= n; ++i) {
      if (spf[i] == 0) {
        spf[i] = i;
        mu[i] = -1;
        phi[i] = i - 1;
        primes.push_back(i);
      }
      for (const auto &p: primes) {
        if (i * p > n) break;
        spf[i * p] = p;
        if (i % p == 0) {
          mu[i * p] = 0;
          phi[i * p] = phi[i] * p;
          break;
        } else {
          mu[i * p] = -mu[i];
          phi[i * p] = phi[i] * phi[p];
        }
      }
    }
  }
} sieve(200005);

auto solve() {
  int K = in;
  vector<pair<int, int>> ab(K);
  int bmin = 1e8;
  REP(i, K) {
    ab[i].first = int(in);
    ab[i].second = int(in);
    chmin(bmin, ab[i].second);
  }
  Mint ans = 0;
  for (int d = 1; d <= bmin; ++d) {
    Mint co = 1;
    REP(i, K) {
      co *= ab[i].second / d - (ab[i].first - 1) / d;
    }
    ans += co * sieve.phi[d];
  }
  Mint den = 1;
  REP(i, K) {
    den *= ab[i].second - (ab[i].first - 1);
  }
  ans = (-ans) / den;
  cout << ans << '\n';
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    (solve());
  }
}
