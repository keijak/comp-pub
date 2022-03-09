#include <bits/stdc++.h>
using Int = long long;
using namespace std;

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

  static  int mod() { return M; }
  static  unsigned umod() { return M; }
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
  ModInt pow(unsigned long long t) const {
    if (_v == 0) {
      return 0;  // corner case: 0^0 = ?
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

template<class T = Mint>
struct Factorials {
  // factorials and inverse factorials.
  std::vector<T> fact, ifact;

  // n: max cached value.
  explicit Factorials(int n) : fact(n + 1), ifact(n + 1) {
    assert(n >= 0);
    assert(n < T::mod());
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) {
      fact[i] = fact[i - 1] * i;
    }
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; --i) {
      ifact[i - 1] = ifact[i] * i;
    }
  }

  // Combination (binomial coefficients)
  T C(Int n, Int k) const {
    if (k < 0 || k > n) return 0;
    return fact[n] * ifact[k] * ifact[n - k];
  }
};

template<typename T>
std::vector<T> pow_seq(int n, int base = 2) {
  assert(n >= 0 and base > 0);
  std::vector<T> p(n + 1);
  p[0] = 1;
  for (int i = 0; i < n; ++i) {
    p[i + 1] = p[i] * base;
  }
  return p;
}

struct BlackBoxDiv2 {
  int count(string front, string side) {
    const int H = (int) std::count(front.begin(), front.end(), 'B');
    const int W = (int) std::count(side.begin(), side.end(), 'B');
    Factorials<Mint> fs(H * W + 10);
    vector<Mint> pow2 = pow_seq<Mint>(H * W + 10, 2);
    Mint ans = 0;
    for (int rb = 0; rb <= H; ++rb) {
      for (int cb = 0; cb <= W; ++cb) {
        int sign = ((rb + cb) & 1) ? -1 : 1;
        ans += sign * fs.C(H, rb) * fs.C(W, cb) * pow2[(H - rb) * (W - cb)];
      }
    }
    return (int) ans.val();
  }
};

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);

  BlackBoxDiv2 solver;
  DUMP(solver.count("BB", "BB"));
  DUMP(solver.count("...", "....."));
  DUMP(solver.count("...", "BBB"));
  DUMP(solver.count("...BB.B....", "B.B.B.B.B"));
  DUMP(solver.count("BBBBB.BBBBBBBBB.BBBBBBB.BBBB.B.BB..BB.B.BBBBBBBBBB",
                    ".B.BBB..BBBBBB.BBBB.B...BBBB.BB.BBBBBBBBBB.....BBB"));
}
