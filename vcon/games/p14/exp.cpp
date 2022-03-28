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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Angel" : "Devil") << endc;
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

map<tuple<Int, Int, int>, bool> memo;

bool solve(Int a, Int b, int parity) {
  if (b == 0) return parity;
  if (a == 0) {
    Int m = (parity ? b + 2 : b) % 4;
    return m == 1 or m == 2;
  }

  auto mkey = tuple(a, b, parity);
  if (auto it = memo.find(mkey); it != memo.end()) {
    return it->second;
  }

  bool ret = false;

  // take 1
  if ((b < 2 or solve(a, b - 2, parity ^ 1)) and solve(a - 1, b - 1, parity ^ 1)) {
    ret = true;
  }
  // take 0
  if (solve(a - 1, b - 1, parity) and (a < 2 or solve(a - 2, b, parity))) {
    ret = true;
  }

  memo[mkey] = ret;
  return ret;
}

int main() {
#if 0
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  Int a = in, b = in;
  print(solve(a, b, 0));
#else
  for (int b = 0; b <= 30; ++b) {
    printf("b=%2d|", b);
    for (int a = 0; a <= 80; ++a) {
      int angel = solve(a, b, 0);
      cout << " " << angel;
    }
    cout << endl;
  }
#endif
}
