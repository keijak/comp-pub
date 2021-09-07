#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
std::ostream &print_seq(const Container &a, std::string sep = " ",
                        std::string ends = "\n", std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template <class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template <typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

struct Input {
  template <typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;
// const int L = 1 << 24;
const int L = 10'000'001;

struct PrimeSieve {
  std::vector<int> spf;  // smallest prime factors table.
  std::vector<int> primes;
  std::vector<int> phi;

  explicit PrimeSieve(int n) : spf(n + 1), phi(n + 1) {
    phi[1] = 1;
    // O(n)
    for (int i = 2; i <= n; ++i) {
      if (spf[i] == 0) {
        spf[i] = i;
        primes.push_back(i);
        phi[i] = i - 1;
      }
      for (const auto &p : primes) {
        if (i * p > n or p > spf[i]) break;
        spf[i * p] = p;
        if (i % p == 0) {
          phi[i * p] = phi[i] * p;  // prime[j] divides i
          break;
        } else {
          phi[i * p] = phi[i] * phi[p];  // prime[j] does not divide i
        }
      }
    }
  }
} const sieve(L);

// O(N log log N)
template <class T>
void divisor_zeta_transform(std::vector<T> &a) {
  const int n = a.size();
  for (auto p : sieve.primes) {
    for (int k = 1; k * p < n; ++k) {
      a[k * p] += a[k];
    }
  }
}

auto main() -> int {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  vector<__int128_t> a(L);
  REP(i, L) a[i] = __int128_t(i) * sieve.phi[i];
  divisor_zeta_transform(a);

  const int t = in;
  REP(test_case, t) {
    const int n = in;
    print((i64)a[n]);
  }
}
