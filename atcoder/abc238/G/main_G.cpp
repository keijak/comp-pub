#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
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

const int L = int(1e6) + 100;

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
} sieve(L);

std::mt19937_64 &PRNG() {
  static std::mt19937_64 engine(std::random_device{}());
  return engine;
}

Uint randint() {
  static std::uniform_int_distribution<Uint> rand(100, std::numeric_limits<Uint>::max());
  return rand(PRNG());
}

auto solve() {
  int n = in, Q = in;
  vector<int> a = in(n);

  vector<Uint> hash1(L), hash2(L);
  for (auto p: sieve.primes) {
    hash1[p] = randint();
    hash2[p] = randint();
  }

  vector<int> pcounter(L, 0);
  vector<Uint> cum_hash(n + 1);
  REP(i, n) {
    Uint delta = 0;
    for (auto[p, k]: sieve.factorize(a[i])) {
      int r = k % 3;
      if (r == 0) continue;
      int pr = pcounter[p] % 3;
      if (r == 1) {
        if (pr == 0) {
          delta ^= hash1[p];
        } else if (pr == 1) {
          delta ^= hash2[p];
        } else {
          delta ^= hash1[p] ^ hash2[p];
        }
        pcounter[p] += 1;
      } else {
        if (pr == 0) {
          delta ^= hash1[p] ^ hash2[p];
        } else if (pr == 1) {
          delta ^= hash1[p];
        } else {
          delta ^= hash2[p];
        }
        pcounter[p] += 2;
      }
      if (pcounter[p] >= 3) pcounter[p] -= 3;
    }
    cum_hash[i + 1] = cum_hash[i] ^ delta;
  }
  REP(i, Q) {
    int l = in, r = in;
    --l;
    print(cum_hash[r] == cum_hash[l]);
  }
  return Void();
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
