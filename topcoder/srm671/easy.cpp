#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <unsigned M>
struct ModInt {
  ModInt(long long val = 0) : _v(0) {
    if (val < 0) {
      long long k = (std::abs(val) + M - 1) / M;
      val += k * M;
    }
    assert(val >= 0);
    _v = val % M;
  }

  static constexpr int mod() { return M; }
  static constexpr unsigned umod() { return M; }
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
    _v = ((unsigned long long)(_v)*a._v) % M;
    return *this;
  }

  ModInt &operator/=(const ModInt &a) { return *this *= a.inv(); }

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
  unsigned _v;  // raw value
};
const unsigned MOD = 1000000007;
using Mint = ModInt<MOD>;

using namespace std;

struct BearCries {
  int count(string message) {
    const int n = message.size();
    int semicolon_count = 0;
    for (char ch : message) {
      if (ch == ';') ++semicolon_count;
    }
    if (semicolon_count % 2 == 1 or semicolon_count == n) {
      return 0;
    }
    vector<vector<vector<Mint>>> dp(
        2, vector<vector<Mint>>(n + 1, vector<Mint>(n + 1, Mint(0))));
    dp[0][0][0] = 1;
    REP(i, n) {
      const bool semicolon = (message[i] == ';');
      REP(j, n + 1) REP(k, n + 1) {
        const auto val = dp[0][j][k];
        if (semicolon) {
          if (j + 1 <= n) dp[1][j + 1][k] += val;
          if (k - 1 >= 0) dp[1][j][k - 1] += val * k;
        } else {
          if (k > 0) dp[1][j][k] += val * k;
          if (j - 1 >= 0 and k + 1 <= n) dp[1][j - 1][k + 1] += val * j;
        }
      }
      swap(dp[0], dp[1]);
      dp[1].assign(n + 1, vector<Mint>(n + 1, Mint(0)));
    }
    return (int)dp[0][0][0].val();
  }
};

int main() {
  BearCries solver;
  auto test = [&](string m) {
    auto ans = solver.count(m);
    cerr << ans << endl;
  };
  test(";_;;_____;");
  test(";;;___;;;");
  test("_;__;");
  test(";_____________________________________;");
  test(";__;____;");
  test(";_;;__;_;;");
}
