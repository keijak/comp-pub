#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<unsigned M>
struct ModInt {
  ModInt(long long val = 0) : _v(0) {
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

  ModInt inv() const {
    return pow(M - 2);
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

using namespace std;

using Mint = ModInt<MOD>;

//#include <boost/multiprecision/cpp_int.hpp>
// using Mint = boost::multiprecision::cpp_rational;

Mint dp[2005][4005][2];

struct DoubleLive {
  int findEV(int B, int H, int T) {
    vector<Mint> minv(B + H + 5);
    REP(i, 1, B + H + 5) minv[i] = Mint(i).inv();

    REP(i, B) REP(j, B + H) REP(k, 2) dp[i][j][k] = 0;
    dp[B - 1][H - 1][0] = 1;

    REP(wounded, 2) {
      for (int r2 = B - 1; r2 >= 0; --r2) {
        for (int r1 = B + H - 2 - r2; r1 >= 0; --r1) {
          if (r1 + r2 + 2 > B + H) continue;
          if (dp[r2][r1][wounded].val() == 0) continue;
          Mint p = dp[r2][r1][wounded];
          Mint total = minv[r2 + r1 + 2];
          if (wounded) {
            if (r1 - 1 >= 0) {
              dp[r2][r1 - 1][1] += p * Mint(r1) * total;
            }
            if (r2 - 1 >= 0) {
              dp[r2 - 1][r1 + 1][1] += p * Mint(r2) * total;
            }
          } else {
            if (r1 - 1 >= 0) {
              dp[r2][r1 - 1][0] += p * Mint(r1) * total;
            }
            if (r2 - 1 >= 0) {
              dp[r2 - 1][r1 + 1][0] += p * Mint(r2) * total;
            }
            dp[r2][r1][1] += p * Mint(1) * total;
          }
        }
      }
    }

    Mint ans = 0;
    REP(i, B) {
      REP(j, B + H - 1) {
        REP(wounded, 2) {
          int consumed = (2 * B + H) - (2 * i + j + 3 - wounded);
          if (consumed == T) {
            Mint res = dp[i][j][wounded] * Mint(i + j + 2) * B * H;
            ans += res;
          }
        }
      }
    }
    return int(ans.val());
  }
};

#include "gtest/gtest.h"

TEST(DoubleLiveTest, Sample) {
  DoubleLive s;
  EXPECT_EQ(s.findEV(4, 3, 1), 571428644);
  EXPECT_EQ(s.findEV(3, 10, 0), 390);
  EXPECT_EQ(s.findEV(1, 2, 2), 111111113);
  EXPECT_EQ(s.findEV(1, 1, 1), 1);
  EXPECT_EQ(s.findEV(1, 1, 2), 0);
  EXPECT_EQ(s.findEV(3, 10, 16), 0);
  EXPECT_EQ(s.findEV(5, 2, 5), 519487272);
  EXPECT_EQ(s.findEV(1807, 1234, 4040), 373880953);
}
