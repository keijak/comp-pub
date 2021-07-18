#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("inline")
#pragma GCC target("avx2")

#include <bits/stdc++.h>

#include <atcoder/twosat>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
inline void read_unsigned(T &ret) {
  int ch = getchar_unlocked();
  for (; isspace(ch); ch = getchar_unlocked());
  for (; isdigit(ch); ch = getchar_unlocked()) {
    ret = (ret * 10) + (ch - '0');
  }
}

// Returns a vector that maps x to x's smallest prime factor (> 1).
// O(n log log n)
std::vector<int> sieve_smallest_prime_factors(int n) {
  std::vector<int> res(n + 1);
  for (int i = 1; i <= n; ++i) res[i] = i;
  for (int i = 2; i <= n; i += 2) res[i] = 2;
  for (int i = 3; i * i <= n; i += 2) {
    if (res[i] != i) continue;
    for (int j = i * i; j <= n; j += i) {
      if (res[j] == j) res[j] = i;
    }
  }
  return res;
}

// Quick factorization with smallest prime factors.
std::vector<std::pair<int, int>> factorize(int n, const std::vector<int> &spf) {
  assert(0 < n and n < int(spf.size()));
  std::vector<std::pair<int, int>> res;
  for (;;) {
    const int p = spf[n];
    if (p == 1) break;
    int count = 0;
    do {
      n /= p;
      ++count;
    } while (n % p == 0);
    res.emplace_back(p, count);
  }
  return res;
}

std::vector<int> primes_upto(int n) {
  std::vector<int> res;
  const auto &spf = sieve_smallest_prime_factors(n);
  for (int i = 2; i <= n; ++i) {
    if (spf[i] == i) res.push_back(i);
  }
  return res;
}

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  int index(T x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  const T &value(int i) const { return values[i]; }
};

const int L = 2000005;

auto solve() {
//  auto spf = sieve_smallest_prime_factors(L);
//  auto primes = primes_upto(L);
//  DUMP(primes.size());
//  Compressed<int> cp(primes);
//  vector<vector<int>> usecount(primes.size(), 0);
//
  int n = 0;
  read_unsigned(n);
  vector<pair<int, int>> ab;
  ab.reserve(n);
  REP(i, n) {
    int a = 0, b = 0;
    read_unsigned(a);
    read_unsigned(b);
    if (a == 1 or b == 1) continue;
    ab.emplace_back(a, b);
  }
  const int m = ab.size();
  atcoder::two_sat ts(m);
  REP(i, m) {
    const auto&[ai, bi] = ab[i];
    REP(j, i + 1, m) {
      const auto&[aj, bj] = ab[j];
      int cc = 0;
      if (std::gcd(ai, aj) != 1) {
        ts.add_clause(i, false, j, false);
        ++cc;
      }
      if (std::gcd(ai, bj) != 1) {
        ts.add_clause(i, false, j, true);
        ++cc;
      }
      if (std::gcd(bi, aj) != 1) {
        ts.add_clause(i, true, j, false);
        ++cc;
      }
      if (std::gcd(bi, bj) != 1) {
        ts.add_clause(i, true, j, true);
        ++cc;
      }
      if (cc == 4) return false;
    }
  }
  return ts.satisfiable();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans ? "Yes" : "No");
  }
}
