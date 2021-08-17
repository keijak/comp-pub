#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
using i64 = long long;

template<unsigned M>
struct ModInt {
  constexpr ModInt(long long val = 0) : _v(0) {
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

  constexpr ModInt operator-() const { return ModInt(umod() - _v); }

  constexpr ModInt &operator+=(const ModInt &a) {
    if ((_v += a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator-=(const ModInt &a) {
    if ((_v += M - a._v) >= M) _v -= M;
    return *this;
  }
  constexpr ModInt &operator*=(const ModInt &a) {
    _v = ((unsigned long long) (_v) * a._v) % M;
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
    auto[g, x, y] = ext_gcd(_v, M);
    assert(g == 1);  // The GCD must be 1.
    return x;

    // Inverse by Fermat's little theorem. M must be prime.
    //
    //     return pow(M - 2);
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
  // Extended Euclidean algorithm.
  // Returns [g, x, y] where g = a*x + b*y = GCD(a, b).
  static constexpr std::array<long long, 3> ext_gcd(int a, int b) {
    if (b == 0) return {a, 1, 0};
    auto res = ext_gcd(b, a % b);  // = (g, x, y)
    res[1] -= (a / b) * res[2];
    std::swap(res[1], res[2]);
    return res;  // = (g, y, x - (a/b)*y)
  }

  unsigned _v;  // raw value
};
const unsigned MOD = 998244353;
using Mint = ModInt<MOD>;

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template<typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}
template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

struct SmallestPrimeFactors {
  std::vector<int> table;  // smallest prime factors table.

  explicit SmallestPrimeFactors(int n) : table(n + 1) {
    for (int i = 1; i <= n; ++i) table[i] = i;
    for (int i = 2; i <= n; i += 2) table[i] = 2;
    for (int i = 3; i * i <= n; i += 2) {
      if (table[i] != i) continue;
      for (int j = i * i; j <= n; j += i) {
        if (table[j] == j) table[j] = i;
      }
    }
  }

  int mobius(int n) {
    assert(0 < n and n < int(table.size()));
    int res = 1;
    while (n > 1) {
      const int p = table[n];
      n /= p;
      res *= -1;
      if (n % p == 0) return 0;
    }
    return res;
  }
};

auto solve() {
  INPUT(int, n, m);
  SmallestPrimeFactors spf(m + 5);
  vector<pair<int, int>> lr(n);
  REP(i, n) {
    INPUT(int, l, r);
    lr[i] = {l, r};
  }

  auto f = [&](int x) -> Mint {
    i64 lb = 0;
    vector<int> width(n);
    REP(i, n) {
      auto[l, r] = lr[i];
      int kl = (l + x - 1) / x;
      int kr = r / x;
      if (kl > kr) return 0;
      lb += kl * x;
      width[i] = kr - kl + 1;
    }
    if (lb > m) return 0;
    int r = (m - lb) / x;
    auto dp = vector(2, vector(r + 1, Mint(0)));
    dp[0][0] = 1;
    REP(i, n) {
      for (int j = 0; j <= r; ++j) {
        dp[1][j] += dp[0][j];
        if (j + width[i] <= r) dp[1][j + width[i]] -= dp[0][j];
      }
      REP(j, r) dp[1][j + 1] += dp[1][j];
      swap(dp[0], dp[1]);
      dp[1].assign(r + 1, 0);
    }
    Mint res = 0;
    for (int j = 0; j <= r; ++j) {
      res += dp[0][j];
    }
    return res;
  };

  Mint ans = 0;
  for (int x = 1; x <= m; ++x) {
    int mu = spf.mobius(x);
    if (mu == 0) continue;
    ans += mu * f(x);
  }
  return ans;

//  auto dp = vector(m + 1, Mint(0));
//  for (int x = m; x >= 1; --x) {
//    dp[x] = f(x);
//    for (int y = 2 * x; y <= m; y += x) {
//      dp[x] -= dp[y];
//    }
//  }
//  return dp[1];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
