#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <boost/multiprecision/cpp_int.hpp>
using BigInt = Int; //boost::multiprecision::cpp_int;
namespace multip = boost::multiprecision;

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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

struct PrimeSieve {
  std::vector<int> spf;  // smallest prime factors table.
  std::vector<int> mu;   // Moebius function
  std::vector<int> primes;

  explicit PrimeSieve(int n) : spf(n + 1), mu(n + 1) {
    mu[1] = 1;
    for (int i = 2; i <= n; ++i) {
      if (spf[i] == 0) {
        spf[i] = i;
        mu[i] = -1;
        primes.push_back(i);
      }
      for (const auto &p: primes) {
        if (i * p > n) break;
        spf[i * p] = p;
        if (i % p == 0) {
          mu[i * p] = 0;
          break;
        } else {
          mu[i * p] = -mu[i];
        }
      }
    }
  }

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

std::vector<Int> divisors(Int n) {
  std::vector<Int> res;
  for (Int k = 1; k * k <= n; ++k) {
    if (n % k != 0) continue;
    res.push_back(k);
    Int q = n / k;
    if (q != k) res.push_back(q);
  }
  //std::sort(res.begin(), res.end());
  return res;
}

std::vector<Int> enumerate_divisors(
    const std::vector<std::pair<Int, int>> &fac) {
  std::vector<Int> res = {1};
  for (const auto &f: fac) {
    Int p;
    int k;
    tie(p, k) = f;
    int sz = res.size();
    for (int i = 0; i < sz; ++i) {
      Int pp = 1;
      for (int j = 0; j < k; ++j) {
        pp *= p;
        res.push_back(res[i] * pp);
      }
    }
  }
  //std::sort(res.begin(), res.end());
  return res;
}

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

struct Solver {
  PrimeSieve sieve;

  Solver() : sieve(1100100) {}

  inline Int asum(Int n) const {
    return n * (n + 1) / 2;
  }

  auto solve() {
    Int n = in;
    BigInt ans = 0;
    for (Int k: enumerate_divisors(sieve.factorize(n))) {
      BigInt co = 0;
      Int r = n / k;
      auto fac = sieve.factorize(r);
      int m = (int) fac.size();
      REP(bits, 1 << m) {
        Int d = 1;
        int mu = 1;
        REP(i, m) if (has_bit(bits, i)) {
            d *= fac[i].first;
            mu *= -1;
          }
        co += asum(r / d) * mu * d;
      }
      ans += co;
    }
    ans *= n;
    cout << ans << '\n';
  }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  Solver solver;
  const int T = in;
  REP(t, T) {
    (solver.solve());
  }
}
