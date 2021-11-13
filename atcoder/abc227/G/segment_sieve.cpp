#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <boost/multiprecision/cpp_int.hpp>
using i256 = boost::multiprecision::checked_int256_t;
// using Uint = boost::multiprecision::checked_uint256_t;
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
  return (int) a.size();
}
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

struct Void {};  // No print.

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, Void>) {
    return std::cout;  // Nothing.
  } else if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;

struct PrimeSieve {
  std::vector<int> spf;  // smallest prime factors table.
  std::vector<int> primes;

  explicit PrimeSieve(int n) : spf(n + 1) {
    // O(n)
    for (int i = 2; i <= n; ++i) {
      if (spf[i] == 0) {
        spf[i] = i;
        primes.push_back(i);
      }
      for (const auto &p: primes) {
        if (i * p > n or p > spf[i]) break;
        spf[i * p] = p;
      }
    }
  }

  inline bool is_prime(int n) const { return spf[n] == n; }

  auto factorize(int n) const {
    assert(0 < n and n < int(spf.size()));
    std::vector<std::pair<Int, int>> res;
    while (n > 1) {
      const int p = spf[n];
      int count = 0;
      do {
        n /= p;
        ++count;
      } while (n % p == 0);
      res.emplace_back(p, count);
    }
    return res;
  }
};

class SegmentSieve {
  Int L, R, M;
  std::vector<Int> spf; // smallest prime factor for [0, sqrt(R)]
  std::vector<std::vector<Int>> factors; // small prime factors of (L+i).
  std::vector<Int> factors_prod;  // product of small prime factors of (L+i).

 public:
  SegmentSieve(Int L, Int R) : L(L), R(R), M(1) {
    while (M * M <= R) M <<= 1;  // Ensure M > sqrt(R).
    spf.resize(M);
    std::iota(spf.begin(), spf.end(), 0);
    factors.resize(R - L);
    factors_prod.assign(R - L, 1);

    for (Int i = 2; i * i < R; ++i) {
      if (spf[i] != i) continue;
      for (Int j = i * i; j < M; j += i) {
        if (spf[j] == j) spf[j] = i;
      }
      for (Int j = (L + i - 1) / i * i; j < R; j += i) {
        Int num = j;
        do {
          if (factors_prod[j - L] >= M) break;
          factors[j - L].push_back(i);
          factors_prod[j - L] *= i;
          num /= i;
        } while (num % i == 0);
      }
    }
  }

  inline bool is_prime(int n) const {
    if (n <= 1) return false;
    if (n < M) return spf[n] == n;
    assert(L <= n and n < R);
    return factors_prod[n - L] == 1;
  }

  std::vector<Int> factorize(Int n) const {
    if (n <= 1) return {};
    assert(n < M or L <= n and n < R);
    std::vector<Int> res;
    if (n >= L) {
      res = factors[n - L];
      n /= factors_prod[n - L];
      if (n > M) {
        res.push_back(n);  // n must be a large prime.
        n = 1;
      }
    }
    while (n > 1) {
      res.push_back(spf[n]);
      n /= spf[n];
    }
    std::sort(res.begin(), res.end());
    return res;
  }
};

auto solve() {
  Int N = in, K = in;
  K = min(K, N - K);
  Int L = N - K + 1;
  SegmentSieve sieve(L, N + 5);

  unordered_map<Int, int> pcount;
  pcount.reserve(1 << 20);
  pcount.max_load_factor(0.25);

  for (Int i = L; i <= N; ++i) {
    auto fac = sieve.factorize(i);
    for (auto p: fac) { ++pcount[p]; }
  }
  for (Int i = 2; i <= K; ++i) {
    auto fac = sieve.factorize(i);
    for (auto p: fac) { --pcount[p]; }
  }
  Mint ans = 1;
  for (auto[p, c]: pcount) {
    if (p <= 1) continue;
    ans *= Mint(c + 1);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
