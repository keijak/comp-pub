#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <boost/multiprecision/cpp_int.hpp>
// using i128 = boost::multiprecision::checked_int128_t;
// using u128 = boost::multiprecision::checked_uint128_t;
using Int = boost::multiprecision::checked_cpp_int;
// using Rational = boost::multiprecision::checked_cpp_rational;
namespace multip = boost::multiprecision;

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) std::size(a);
}

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

template<typename T>
struct Kitamasa {
  const int k;

  // Initial k elements (a[1] .. a[k]).
  const std::vector<T> a_;

  // Coefficients of the recurrence.
  //   a[n] = coeff[1]*a[n-1] + coeff[2]*a[n-2] + ... + coeff[k]*a[n-k].
  const std::vector<T> coeff_;

  explicit Kitamasa(std::vector<T> a, std::vector<T> c)
      : k(a.size()), a_(a), coeff_(std::move(c)) {}

  // n: 1-indexed.
  T nth(long long n) const {
    assert(n >= 1);
    auto x = nth_coeff(n);
    T res = 0;
    for (int i = 0; i < k; ++i) {
      res += x[i] * a_[i];
    }
    return res;
  }

 private:
  // n: 1-indexed.
  std::vector<T> nth_coeff(long long n) const {
    if (n <= k) {
      std::vector<T> res(k, 0);
      res[n - 1] = 1;
      return res;
    }
    // Doubling.
    if (n & 1) {
      // f(n-1) => f(n)
      return next(nth_coeff(n - 1));
    } else {
      // f(n/2) => f(n)
      std::vector<T> s = nth_coeff(n >> 1);
      std::vector<T> t = s;
      std::vector<T> res(k, 0);
      for (int i = 0; i < k; ++i) {
        t = next(std::move(t));
        for (int j = 0; j < k; ++j) {
          res[j] += t[j] * s[i];
        }
      }
      return res;
    }
  }

  std::vector<T> next(std::vector<T> x) const {
    std::vector<T> res(k, 0);
    for (int i = 1; i < k; ++i) {
      res[i] = x[i - 1];
    }
    for (int i = 0; i < k; ++i) {
      res[i] += x[k - 1] * coeff_[k - 1 - i];
    }
    return res;
  }
};

template<int sign = 1>
struct Infinity {
  template<typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned_v<T>,
                  "must be positive in an unsigned type");
    if constexpr (std::numeric_limits<T>::has_infinity) {
      return T(sign) * std::numeric_limits<T>::infinity();
    } else {
      return T(sign) * (std::numeric_limits<T>::max() / T(2));
    }
  }
  constexpr Infinity<sign * -1> operator-() const { return {}; }
  template<typename T>
  friend bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template<typename T>
  friend bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
constexpr Infinity<> INF;

Mint fib(i64 n) {
  vector<Mint> a = {1, 1}, c = {1, 1};
  Kitamasa<Mint>(a, c);
  return km.nth(n);
}

auto solve(i64 n, i64 K) {
  if (K == 1) {
    return fib(n + 2);
  }
  auto dp = vector(n, vector(n + 1, vector(K + 1, Mint(0))));
  for (int x = 0; x <= K; ++x) {
    dp[0][1][x] = 1;
  }
  for (int i = 1; i < n; ++i) {
    for (int x = 0; x <= K; ++x) {
      for (int k = 0; k <= K; ++k) {
        dp[0][1][k] = 1;
      }
      for (int w = 1; w <= i; ++w) {
        for (int k = 0; k <= K; ++k) {
          int k2 = min(k, x);
          if ((w + 1) * k2 <= K) {
            dp[i][w + 1][k2] += dp[i - 1][w][k];
          }
        }
      }
    }
  }

}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int t = 1;
  REP(test_case, t) {
    INPUT(i64, n, K);
    auto ans = solve(n, K);
    print(ans);
  }
}
