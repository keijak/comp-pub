// #define NDEBUG
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
};

PrimeSieve sieve(2000100);

vector<vector<int>> solve3() {
  vector<int> g(9);
  REP(i, 9) g[i] = i + 1;
  do {
    auto ans = vector(3, vector(3, 0));
    REP(i, 9) {
      ans[i / 3][i % 3] = g[i];
    }
    bool ok = [&]() -> bool {
      REP(i, 3) {
        REP(j, 2) {
          if (sieve.is_prime(ans[i][j] + ans[i][j + 1])) return false;
          if (sieve.is_prime(ans[j][i] + ans[j + 1][i])) return false;
        }
      }
      return true;
    }();
    if (ok) return ans;
  } while (next_permutation(ALL(g)));
  assert(false);
}

vector<vector<int>> solve(const int n) {
  if (n == 3) return solve3();
  const int n2 = n * n;
  // non-prime pairs
  vector<pair<int, int>> npp;
  set<int> odds, evens;
  for (int i = 1; i <= n2; i += 2) {
    odds.insert(i);
  }
  for (int x = 2; x <= n2; x += 2) {
    if (ssize(npp) == n + 5) break;
    for (int y: odds) {
      if (n >= 5 and n % 2 == 1 and (x == 4 or y == 5)) continue;
      if (not sieve.is_prime(x + y)) {
        npp.emplace_back(x, y);
        odds.erase(y);
        break;
      }
    }
  }
  DUMP(npp);
  //assert(ssize(npp) == n);

  auto ans = vector(n, vector(n, 0));
  odds.clear();
  evens.clear();
  for (int i = 1; i <= n2; ++i) {
    if (i & 1) {
      odds.insert(i);
    } else {
      evens.insert(i);
    }
  }
  if (n % 2 == 0) {
    REP(i, n) {
      auto [e, o] = npp[i];
      ans[n / 2 - 1][i] = e;
      ans[n / 2][i] = o;
      assert(evens.count(e));
      evens.erase(e);
      assert(odds.count(o));
      odds.erase(o);
    }
    REP(i, n) {
      REP(j, n) {
        if (i < n / 2 - 1) {
          auto it = evens.begin();
          ans[i][j] = *it;
          evens.erase(it);
        } else if (i > n / 2) {
          auto it = odds.begin();
          ans[i][j] = *it;
          odds.erase(it);
        }
      }
    }
  } else {
    ans[n / 2][n / 2] = 5;
    ans[n / 2][n / 2 - 1] = 4;
    ans[n / 2 - 1][n / 2] = 10;
    ans[n / 2 + 1][n / 2 - 1] = 11;
    odds.erase(5);
    odds.erase(11);
    evens.erase(4);
    evens.erase(10);
    REP(i, n / 2 - 1) {
      auto [e, o] = npp.back();
      npp.pop_back();
      ans[n / 2][i] = e;
      ans[n / 2 + 1][i] = o;
      evens.erase(e);
      odds.erase(o);
    }
    REP(i, n / 2 + 1, n) {
      auto [e, o] = npp.back();
      npp.pop_back();
      ans[n / 2 - 1][i] = e;
      ans[n / 2][i] = o;
      evens.erase(e);
      odds.erase(o);
    }
    REP(i, n) {
      REP(j, n) {
        if (ans[i][j] != 0) continue;
        if (i < n / 2) {
          auto it = evens.begin();
          ans[i][j] = *it;
          evens.erase(it);
        } else if (i > n / 2) {
          auto it = odds.begin();
          ans[i][j] = *it;
          odds.erase(it);
        }
      }
    }
  }
  return ans;
}

int main() {
  init_();
  const int n = in;
//  for (int n = 4; n <= 10; ++n) {
//    test_case(n, 10);

  auto ans = solve(n);
  assert(ssize(ans) == n);
  set<int> s;
  REP(i, n) {
    assert(ssize(ans[i]) == n);
    out_seq(ans[i]);
    for (int x: ans[i]) {
      assert(1 <= x and x <= n * n);
      s.insert(x);
    }
  }
  assert(ssize(s) == n * n);
//  }
  exit_();
}
