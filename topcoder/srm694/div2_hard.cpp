#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
  friend ModInt operator+(const ModInt &a, const ModInt &b) {
    return ModInt(a) += b;
  }
  friend ModInt operator-(const ModInt &a, const ModInt &b) {
    return ModInt(a) -= b;
  }
  friend ModInt operator*(const ModInt &a, const ModInt &b) {
    return ModInt(a) *= b;
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
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}

using namespace std;

struct UpDownNess {
  int count(int N, int K) {
    vector<vector<Mint>> dp(N + 1, vector<Mint>(K + 1, Mint(0)));
    if (1 <= N) dp[1][0] = 1;
    if (2 <= N) dp[2][0] = 2;
    for (int i = 3; i <= N; ++i) {
      for (int j = 0; j <= K; ++j) {
        dp[i][j] += dp[i - 1][j] * 2;
        for (int p = 1; p < i - 1; ++p) {
          int q = i - 1 - p;
          if (j + p * q <= K) dp[i][j + p * q] += dp[i - 1][j];
        }
      }
    }
    return dp[N][K].val();
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  vector<array<int, 3 >> tests = {
      {3, 1, 2},
      {3, 0, 4},
      {4, 3, 4},
      {19, 19, 24969216},
      {50, 2000, 116596757},
  };
  for (auto[a, b, c]: tests) {
    UpDownNess solver;
    //DUMP(a, b, c, solver.count(a, b));
  }
}
