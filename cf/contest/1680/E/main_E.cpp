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
void exit_() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

Int solve() {
  const int n = in;
  array<vector<int>, 2> S;
  REP(i, 2) {
    string s = in;
    S[i].assign(n, 0);
    REP(j, n) {
      S[i][j] = (s[j] == '*');
    }
  }

  int total_chips = 0;
  REP(i, n) {
    REP(j, 2) if (S[j][i]) ++total_chips;
  }
  if (total_chips <= 1) {
    return 0;
  }
  Int ans = kBigVal<Int>;
  REP(iteration, 2) {
    int lmost = -1;
    REP(i, n) {
      if (S[0][i] or S[1][i]) {
        lmost = i;
        break;
      }
    }
    assert(lmost >= 0);

    int rem = total_chips;
    array<Int, 2> dp = {kBigVal<Int>, kBigVal<Int>};
    if (S[0][lmost] and S[1][lmost]) {
      dp[0] = dp[1] = 1;
      rem -= 1;
    } else if (S[0][lmost]) {
      dp[0] = 0;
    } else {
      dp[1] = 0;
    }
    if (rem == 1) {
      chmin(ans, min(dp[0], dp[1]));
    } else {
      for (int i = lmost + 1; i < n; ++i) {
        array<Int, 2> ndp = {kBigVal<Int>, kBigVal<Int>};
        chmin(ndp[0], dp[1] + 2);
        if (not S[1][i]) {
          chmin(ndp[0], dp[0] + 1);
        } else {
          chmin(ndp[0], dp[0] + 2);
        }

        chmin(ndp[1], dp[0] + 2);
        if (not S[0][i]) {
          chmin(ndp[1], dp[1] + 1);
        } else {
          chmin(ndp[1], dp[1] + 2);
        }

        DUMP(i, ndp);
        int chips = S[0][i] + S[1][i];
        rem -= chips;
        if (rem <= 1) {
          chmin(ans, min(ndp[0], ndp[1]));
          break;
        }
        dp = ndp;
      }
    }
    reverse(ALL(S[0]));
    reverse(ALL(S[1]));
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    DUMP(t);
    print(solve());
  }
  exit_();
}
