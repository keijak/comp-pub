#include<vector>
#include<cstdio>
#include<algorithm>
#include<bitset>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) x.begin(), x.end()
typedef long long Int;
typedef unsigned long long Uint;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }

//#ifdef MY_DEBUG
//#include "debug_dump.hpp"
//#else
//#define DUMP(...)
//#define cerr if(false)cerr
//#endif

using namespace std;

struct LinearSieve {
  std::vector<int> spf;  // smallest prime factors.
  std::vector<int> mu;   // Moebius function
  std::vector<int> primes;

  explicit LinearSieve(int n) : spf(n + 1), mu(n + 1) {
    mu[1] = 1;
    for (int i = 2; i <= n; ++i) {
      if (spf[i] == 0) {
        spf[i] = i;
        mu[i] = -1;
        primes.push_back(i);
      }
      for (std::vector<int>::iterator it = primes.begin(); it != primes.end(); ++it) {
        int p = *it;
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

  std::vector<std::pair<Int, int> > factorize(int n) const {
    //assert(0 < n and n < int(spf.size()));
    std::vector<std::pair<Int, int> > res;
    while (n > 1) {
      const int p = spf[n];
      int count = 0;
      do {
        n /= p;
        ++count;
      } while (n % p == 0);
      res.push_back(std::pair<Int, int>(p, count));
    }
    return res;
  }
};

// Enumerates divisors from prime factorization.
// O(d(n)) + sorting
std::vector<Int> enumerate_divisors(
    const std::vector<std::pair<Int, int> > &fac,
    std::vector<Int> &res) {
  res.push_back(1);
  for (int l = 0; l < (int) fac.size(); ++l) {
    Int p = fac[l].first;
    int k = fac[l].second;
    int sz = (int) res.size();
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
T comb(Int n, Int k) {
  if (k < 0 or k > n) return 0;
  if (k > n - k) k = n - k;
  T nume = 1, deno = 1;
  for (int i = 0; i < k; ++i) {
    nume *= T(n - i);
    deno *= T(i + 1);
  }
  return nume / deno;
}

struct Solver {
  LinearSieve sieve;

  Solver() : sieve(10000) {
  }

  void solve(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
      scanf("%d", &a[i]);
    }
    const int amax = *max_element(ALL(a));
    vector<int> acnt(amax + 1);
    REP(i, n) ++acnt[a[i]];
    vector<int> cnt(amax + 1);
    for (int i = 1; i <= amax; ++i) {
      for (int j = i; j <= amax; j += i) {
        cnt[i] += acnt[j];
      }
    }

    Uint ans = 0;
    for (int d = 1; d <= amax; ++d) {
      Uint mu = sieve.mu[d];
      if (mu == 0) continue;
      ans += mu * comb<Uint>(cnt[d], 4);
    }
    printf("%llu\n", ans);
  }
};

int main() {
  Solver solver;
  int n;
  while (scanf("%d", &n) >= 0) {
    solver.solve(n);
  }
}
