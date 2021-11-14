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
  std::vector<int> spf;  // smallest prime factors_ table.
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
  Int L, R, max_spf_;
  std::vector<Int> spf_; // smallest prime factor for [0, max_spf_]
  std::vector<std::vector<Int>> factors_; // small prime factors_ of (L+i).
  std::vector<Int> factors_prod_;  // product of small prime factors_ of (L+i).

 public:
  SegmentSieve(Int l, Int r, Int m = 1) : L(l), R(r), max_spf_(m) {
    while (__int128_t(max_spf_) * max_spf_ <= R) max_spf_ *= 2;  // Ensure max_spf > sqrt(R).
    spf_.resize(max_spf_ + 1);
    std::iota(spf_.begin(), spf_.end(), 0);
    factors_.resize(R - L);
    factors_prod_.assign(R - L, 1);

    for (Int i = 2; i * i < R; ++i) {
      if (spf_[i] != i) continue;
      for (Int j = i * i; j <= max_spf_; j += i) {
        if (spf_[j] == j) spf_[j] = i;
      }
      for (Int j = (L + i - 1) / i * i; j < R; j += i) {
        Int num = j;
        do {
          //if (factors_prod_[j - L] > max_spf_) break;
          factors_[j - L].push_back(i);
          factors_prod_[j - L] *= i;
          num /= i;
        } while (num % i == 0);
      }
    }
  }

  inline bool is_prime(int n) const {
    if (n <= 1) return false;
    if (n <= max_spf_) return spf_[n] == n;
    assert(L <= n and n < R);
    auto p = factors_prod_[n - L];
    return p == 1 or p == n;
  }

  std::vector<Int> factorize(Int n) const {
    if (n <= 1) return {};
    assert(n <= max_spf_ or (L <= n and n < R));
    std::vector<Int> res;
    if (n >= L) {
      res = factors_[n - L];
      n /= factors_prod_[n - L];
      if (n > max_spf_) {
        res.push_back(n);  // n must be a large prime.
        n = 1;
      }
    }
    while (n > 1) {
      res.push_back(spf_[n]);
      n /= spf_[n];
    }
    std::sort(res.begin(), res.end());
    return res;
  }
};

auto solve() {
  Int N = in, K = in;
  K = min(K, N - K);
  Int L = N - K + 1;
  SegmentSieve sieve(L, N + 1, Int(1e6 + 10));
  unordered_map<Int, int> pcount;
  for (Int i = L; i <= N; ++i) {
    for (auto p: sieve.factorize(i)) { ++pcount[p]; }
  }
  for (Int i = 2; i <= K; ++i) {
    for (auto p: sieve.factorize(i)) { --pcount[p]; }
  }
  Mint ans = 1;
  for (auto[p, c]: pcount) {
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
