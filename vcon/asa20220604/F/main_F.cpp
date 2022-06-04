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
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

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

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

void exit_() {
#ifdef MY_DEBUG
  std::string unused;
  assert(not(std::cin >> unused));  // No input is left behind.
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

inline void init_test_case(int t, int T) {
#ifdef MY_DEBUG
  if (T > 1) {
    std::cerr << "\033[35m=== case " << t << " of " << T << " ===\033[0m"
              << std::endl;
  }
#endif
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

auto solve() {
  int n = in;
  vector<int> T = in(n);
  vector<int> V = in(n);
  int tsum = accumulate(ALL(T), 0);
  vector<int> vceil(tsum * 2 + 1, kBigVal<int>);
  vceil[0] = 0;
  vceil[tsum * 2] = 0;
  {
    int t = 0;
    REP(i, n) {
      REP(j, T[i] * 2 + 1) {
        chmin(vceil[t + j], V[i] * 2);
      }
      t += T[i] * 2;
    }
  }
  for (int h = 1; h <= 205; ++h) {
    for (int t = 0; t <= tsum * 2; ++t) {
      if (t - 1 >= 0) {
        chmin(vceil[t - 1], vceil[t] + 1);
      }
      if (t + 1 <= tsum * 2) {
        chmin(vceil[t + 1], vceil[t] + 1);
      }
    }
  }
  Int ans = 0;
  for (int t = 0; t < tsum * 2; ++t) {
    ans += (vceil[t] + vceil[t + 1]);
  }
  print(Real(ans) / 8.0);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    init_test_case(t, T);
    (solve());
  }
  exit_();
}
