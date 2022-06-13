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
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
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
    return std::cout << (x ? "YES" : "NO") << endc;
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

#ifdef MY_DEBUG
void test_case(int t, int T) {
  if (T <= 1) return;
  std::cerr << "\033[35m=== case " << t << " of " << T << " ===\033[0m"
            << std::endl;
}
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define test_case(...)
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

auto solve() {
  const int n = in;
  string S = in, T = in;
  assert(ssize(S) == n);
  assert(ssize(T) == n);
  int sb = std::count(ALL(S), 'b');
  int tb = std::count(ALL(T), 'b');
  if (sb != tb) return false;
  int i = n - 1, j = n - 1;
  int bcnt = 0;
  while (bcnt < sb) {
    while (i >= 0 and S[i] != 'b') --i;
    while (j >= 0 and T[j] != 'b') --j;
    if (i == j) {
      --i, --j;
    } else if (i < j) {
      int k = i;
      while (k < j and S[k + 1] == 'c') {
//        swap(S[k], S[k + 1]);
        ++k;
      }
      int r = j;
      while (r > k and T[r - 1] == 'c') {
//        swap(T[r], T[r - 1]);
        --r;
      }
      if (k != r) return false;
      swap(S[i], S[k]);
      swap(T[j], T[r]);
      --i;
      j = r - 1;
    } else {
      int k = j;
      while (k < i and T[k + 1] == 'a') {
        //swap(T[k], T[k + 1]);
        ++k;
      }
      int r = i;
      while (r > k and S[r - 1] == 'a') {
        //swap(S[r], S[r - 1]);
        --r;
      }
      if (k != r) return false;
      swap(T[j], T[k]);
      swap(S[i], S[r]);
      --j;
      i = r - 1;
    }
    ++bcnt;
  }
  return S == T;
}

int main() {
  init_();
  const int T = in;
  REP(t, T) {
    test_case(t, T);
    print(solve());
  }
  exit_();
}
