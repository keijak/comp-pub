#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::static_modint<10000>;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    assert(std::cin >> x);
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) assert(std::cin >> x);
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

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

void exit_() {
#ifdef MY_DEBUG
  std::string unused;
  assert(not(std::cin >> unused));
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

void decrement(string &a) {
  for (int i = ssize(a) - 1; i >= 0; --i) {
    if (a[i] != '0') {
      a[i]--;
      return;
    }
    a[i] = '9';
  }
}

auto solve() {
  string A = in, B = in;
  const int M = in;
  decrement(A);

  auto f = [&](const string &S) -> Mint {
    const int n = ssize(S);
    auto dp = vector(n + 1, vector(2, vector(2, vector(11, vector(3, vector(M, Mint(0)))))));
    dp[0][0][0][0][2][0] = 1;
    REP(i, n) {
      int ud = S[i] - '0';
      REP(less, 2) {
        REP(nonzero, 2) {
          REP(ld, 10) {
            REP(down, 3) {
              REP(s, M) {
                Mint val = dp[i][less][nonzero][ld][down][s];
                if (val.val() == 0) continue;
                for (int d = 0; d <= 9; ++d) {
                  bool less2 = less;
                  bool nonzero2 = nonzero;
                  if (not less and d > ud) continue;
                  if (not less and d < ud) less2 = true;
                  if (not nonzero and d != 0) nonzero2 = true;
                  const int s2 = (s * 10 + d) % M;
                  if (not nonzero) {
                    dp[i + 1][less2][nonzero2][d][2][s2] += val;
                  } else {
                    if (down != 0 and d > ld) {
                      dp[i + 1][less2][nonzero2][d][0][s2] += val;
                    }
                    if (down != 1 and d < ld) {
                      dp[i + 1][less2][nonzero2][d][1][s2] += val;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    Mint ans = 0;
    REP(less, 2) {
      REP(d, 10) {
        REP(down, 3) {
          ans += dp[n][less][1][d][down][0];
        }
      }
    }
    return ans;
  };

  Mint ans = f(B) - f(A);
  print(ans);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}
