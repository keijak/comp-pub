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

void exit_() {
  std::string unused;
  assert(not(std::cin >> unused));
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

bool solve() {
  int a = in, b = in, ab = in, ba = in;
  string s = in;
  const int n = ssize(s);
  DUMP(s, a, b, ab, ba);
  REP(i, n - 1) {
    if (ab > 0 and s[i] == 'A' and s[i + 1] == 'B' and (i == 0 or s[i - 1] != 'B')
        and (i + 1 == n - 1 or s[i + 2] != 'A')) {
      --ab;
      s[i] = s[i + 1] = '_';
    } else if (ba > 0 and s[i] == 'B' and s[i + 1] == 'A' and (i == 0 or s[i - 1] != 'A')
        and (i + 1 == n - 1 or s[i + 2] != 'B')) {
      --ba;
      s[i] = s[i + 1] = '_';
    }
  }
  DUMP(s, a, b, ab, ba);
  REP(i, n - 1) {
    if (ab > 0 and s[i] == 'A' and s[i + 1] == 'B') {
      --ab;
      s[i] = s[i + 1] = '_';
    } else if (ba > 0 and s[i] == 'B' and s[i + 1] == 'A') {
      --ba;
      s[i] = s[i + 1] = '_';
    }
  }
  DUMP(s, a, b, ab, ba);
  if (ab > 0 or ba > 0) return false;
  REP(i, n) {
    if (a > 0 and s[i] == 'A') {
      --a;
      s[i] = '_';
    }
    if (b > 0 and s[i] == 'B') {
      --b;
      s[i] = '_';
    }
  }
  DUMP(s, a, b, ab, ba);
  if (a > 0 or b > 0) return false;
  DUMP(s);
  REP(i, n) {
    assert(s[i] == '_');
  }
  return true;
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
