#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;

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

using namespace std;

struct PrimeSieve {
  std::vector<int> spf;  // smallest prime factors table.
  std::vector<int> primes;
  std::vector<int> mu;

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

  int moebius(int n) const {
    assert(0 < n and n < int(spf.size()));
    int res = 1;
    while (n > 1) {
      const int p = spf[n];
      n /= p;
      if (n % p == 0) return 0;
      res *= -1;
    }
    return res;
  }
};

std::vector<Int> enumerate_divisors(
    const std::vector<std::pair<Int, int>> &fac) {
  std::vector<Int> res = {1};
  for (auto f: fac) {
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
  // std::sort(res.begin(), res.end());
  return res;
}

#include <boost/multiprecision/cpp_int.hpp>
using BigInt = boost::multiprecision::cpp_int;
namespace multip = boost::multiprecision;

Int c3(int n) {
  if (n < 3) return 0;
  Int ret = n;
  ret *= n - 1;
  ret *= n - 2;
  ret /= 6;
  return ret;
}

template<class T>
void divisor_moebius_transform(std::vector<T> &f,
                               const std::vector<int> &primes) {
  const int n = f.size();
  for (auto p: primes) {
    if (p >= n) break;
    for (int k = (n - 1) / p; k > 0; --k) {
      f[k * p] -= f[k];
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int n = in;
  vector<int> a = in(n);
  int amax = *max_element(ALL(a));
  PrimeSieve sieve(amax + 10);
  vector<int> freq(amax + 10);
  REP(i, n) {
    auto fac = sieve.factorize(a[i]);
    for (auto d: enumerate_divisors(fac)) ++freq[d];
  }

  vector<int> f(amax + 10);
  f[1] = 1;
  divisor_moebius_transform(f, sieve.primes);

  Int ans = 0;
  for (int d = 1; d <= amax; ++d) {
    int mu = f[d];
    if (mu == 0) continue;
    ans += mu * c3(freq[d]);
  }
  cout << ans << endl;
}
