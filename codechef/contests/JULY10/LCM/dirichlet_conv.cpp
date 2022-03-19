#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
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

constexpr int L = 4000005;
uint32_t spf[L];  // smallest prime factors.
uint32_t mu[L];
uint32_t F[L];
uint32_t G[L];
uint32_t PF[L + 1];

struct LinearSieve {
  std::vector<uint32_t> primes;

  explicit LinearSieve(uint32_t n) {
    mu[1] = 1;
    for (uint32_t i = 2; i <= n; ++i) {
      if (spf[i] == 0) {
        spf[i] = i;
        mu[i] = -1;
        primes.push_back(i);
      }
      for (const uint32_t &p: primes) {
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
    for (uint32_t i = 0; i <= n; ++i) {
      F[i] = i * i * mu[i];
      G[i] = i * mu[i] * mu[i];
    }
    dirichlet_convolution(n);
    for (uint32_t i = 0; i <= n; ++i) {
      PF[i + 1] = PF[i] + F[i];
    }
  }

  void dirichlet_convolution(uint32_t n) {
    for (int p: primes) {
      for (int i = n / p; i > 0; --i) {
        const int to = i * p;
        for (int x = i, y = p;; x /= p, y *= p) {
          F[to] += F[x] * G[y];
          if (x % p != 0) break;
        }
      }
    }
  }
};

static constexpr Int kMask = (1 << 30) - 1;

inline uint32_t asum(Uint n) {
  return uint32_t((n * (n + 1)) >> 1);
}

struct Solver {
  LinearSieve sieve;
  Solver() : sieve(4000004) {}

  auto solve() {
    uint32_t a = in, b = in;
    if (a > b) swap(a, b);
    uint32_t ans = 0;
    Int al = 1;
    while (al <= a) {
      Int aval = a / al;
      Int ar = a / aval + 1;
      uint32_t cob = 0;
      uint32_t bl = al;
      while (bl <= b) {
        if (bl >= ar) break;
        Int bval = b / bl;
        Int br = b / bval + 1;
        cob += (PF[std::min<Int>(ar, br)] - PF[bl]) * asum(bval);
        bl = br;
      }
      ans += cob * asum(aval);
      al = ar;
    }
    ans &= kMask;
    print(ans);
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
