// #define NDEBUG
#include <bits/stdc++.h>
#include <atcoder/mincostflow>

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

auto solve() {
  int n = in;
  vector<int> A(n);
  vector<Int> B(n);
  REP(i, n) {
    A[i] = int(in);
    B[i] = int(in);
  }

  int amax = *max_element(ALL(A));
  static PrimeSieve sieve(amax * 2 + 100);

  auto gmat = vector(n, vector(n, 0));

  REP(i, n) {
    REP(j, n) {
      if (j == i) continue;
      int s = A[i] + A[j];
      if (sieve.is_prime(s)) {
        gmat[i][j] = gmat[j][i] = 1;
      }
    }
  }

  atcoder::mcf_graph<Int, int> mg(n + 2);
  optional<int> e1;
  const int kSource = n;
  const int kSink = n + 1;
  REP(v, n) {
    if (A[v] % 2 == 0) {
      mg.add_edge(kSource, v, B[v], 0);
    } else {
      bool to_one = A[v] == 1;
      int e = mg.add_edge(v, kSink, B[v], int(to_one));
      if (to_one) {
        e1 = e;
      }
    }
  }
  REP(u, n) {
    if (A[u] % 2 == 1) continue;
    REP(v, n) {
      if (A[v] % 2 == 0) continue;
      if (not gmat[u][v]) continue;
      mg.add_edge(u, v, B[u] + B[v], 0);
    }
  }
  auto [ans, cost] = mg.flow(kSource, kSink);
  if (e1.has_value()) {
    const auto &edge = mg.get_edge(e1.value());
    Int rem = max(edge.cap - edge.flow, 0LL);
    ans += rem / 2;
  }
  out(ans);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
