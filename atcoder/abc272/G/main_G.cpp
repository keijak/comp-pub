// #define NDEBUG
#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>

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
constexpr T kBig = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

std::mt19937_64 &PRNG() {
  static std::mt19937_64 engine(std::random_device{}());
  return engine;
}

template<class T>
T majority_vote(const std::vector<T> a) {
  T m = a.at(0);
  int c = 0;
  for (const T &x: a) {
    if (c == 0) {
      m = x;
      ++c;
    } else if (x == m) {
      ++c;
    } else {
      --c;
    }
  }
  return m;
}

// O(n) sieve
struct PrimeSieve {
  std::vector<int> spf;  // smallest prime factors table.
  std::vector<int> primes;

  explicit PrimeSieve(int n) : spf(n + 1) {
    for (int i = 2; i <= n; ++i) {
      if (spf[i] == 0) {
        spf[i] = i;
        primes.push_back(i);
      }
      for (const auto &p: primes) {
        if (i * p > n) break;
        spf[i * p] = p;
        if (i % p == 0) break;
      }
    }
  }

  inline bool is_prime(int n) const { return spf[n] == n; }
} sieve(1000000);

std::vector<Int> divisor(Int n) {
  std::vector<Int> res;

  if (n <= 1000000 and sieve.is_prime(n)) {
    res.push_back(n);
    return res;
  }
  if ((n & 3) == 0) {
    res.push_back(4);
    Int q = n / 4;
    if (q != 4) res.push_back(q);
  }
  for (Int k = 1; k * k <= n; k += 2) {
    if (n % k != 0) continue;
    res.push_back(k);
    Int q = n / k;
    if (q != k) res.push_back(q);
    if (ssize(res) >= 5) break;
  }
  return res;
}

auto solve() -> optional<int> {
  const int n = in;
  vector<int> a = in(n);
  sort(ALL(a));

  unordered_set < Int > checked;
  auto check = [&](Int z) -> bool {
    if (checked.count(z)) return false;
    checked.insert(z);

    vector<int> b = a;
    REP(i, n) b[i] %= z;
    const int m = majority_vote(b);
    const int cnt = std::count(ALL(b), m);
    return cnt > n / 2;
  };

  REP(i, n) {
    int j = (i + 1) % n;
    Int d = abs(a[i] - a[j]);
    if (d <= 2) continue;
    auto divs = divisor(d);
    for (auto z: divs) {
      if (z <= 2) continue;
      if (check(z)) return z;
    }
  }

  return nullopt;
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    auto res = solve();
    out(res.value_or(-1));
  }
  exit_();
}
