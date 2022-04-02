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
void exit() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

auto solve() {
  string S = in, T = in;
  int n = ssize(S), m = ssize(T);
  auto dp = vector(n + 1, vector(m + 1, 0));

  vector<int> snext(n, n), tnext(m, m);
  vector<int> lastindex(26, n);
  for (int i = n - 1; i >= 0; --i) {
    REP(j, 26) {
      if (S[i] == 'a' + j) continue;
      chmin(snext[i], lastindex[j]);
    }
    lastindex[S[i] - 'a'] = i;
  }
  lastindex.assign(26, m);
  for (int i = m - 1; i >= 0; --i) {
    REP(j, 26) {
      if (T[i] == 'a' + j) continue;
      chmin(tnext[i], lastindex[j]);
    }
    lastindex[T[i] - 'a'] = i;
  }
  DUMP(snext);
  DUMP(tnext);

  for (int i = n - 1; i >= 0; --i) {
    for (int j = m - 1; j >= 0; --j) {
      if (S[i] != T[j]) {
        chmax(dp[i][j], dp[i + 1][j + 1] + 1);
      } else {
        int i2 = snext[i];
        if (i2 < n) {
          chmax(dp[i][j], dp[i2 + 1][j + 1] + 1);
        }
        int j2 = tnext[j];
        if (j2 < m) {
          chmax(dp[i][j], dp[i + 1][j2 + 1] + 1);
        }
      }
    }
  }
  print(dp[0][0]);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit();
}
