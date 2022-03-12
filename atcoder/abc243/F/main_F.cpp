#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;
#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}
#include <boost/rational.hpp>
#include <boost/multiprecision/cpp_int.hpp>
//using Mint = boost::multiprecision::cpp_rational;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

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

template<class T = unsigned long long>
std::vector<std::vector<T>> comb_table(int n) {
  std::vector<std::vector<T>> C(n + 1, std::vector<T>(n + 1, 0));
  for (int i = 0; i <= n; ++i) {
    C[i][0] = C[i][i] = 1;
  }
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j < i; ++j) {
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
  }
  return C;
}

Mint pow(Mint b, int k) {
  return b.pow(k);
//  Mint ret = 1;
//  REP(i, k) ret *= b;
//  return ret;
}

auto solve() {
  int n = in, m = in, K = in;
  vector<int> W = in(n);
  Mint wsum = accumulate(ALL(W), Mint(0));
  Mint wden = Mint(1) / wsum;
  vector<Mint> P(n);
  REP(i, n) {
    P[i] = W[i] * wden;
  }
  auto C = comb_table<Mint>(1000);

  auto dp = vector(n + 1, vector(m + 1, vector(K + 1, Mint(0))));
  dp[0][0][0] = 1;
  REP(i, n) {
    Mint p = P[i];
    REP(j, m + 1) {
      REP(k, K + 1) {
        if (dp[i][j][k] == 0) continue;
        auto now = dp[i][j][k];
        REP(x, K - k + 1) {
          int j2 = (x > 0) ? (j + 1) : j;
          int k2 = k + x;
          if (j2 <= m and k2 <= K) {
            dp[i + 1][j2][k2] += now * C[K - k][x] * pow(p, x);
          }
        }
      }
    }
  }
  return dp[n][m][K];
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    print(solve());
  }
}
