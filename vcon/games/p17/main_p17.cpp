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
    return std::cout << (x ? "snuke" : "rng") << endc;
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

vector<vector<optional<bool>>> memo;

bool labo(Int x, Int y) {
  if (x < y) swap(x, y);
  if (y == 1) {
    return x % 2 == 0;
  }
  if (memo[x][y]) {
    return *memo[x][y];
  }
  bool win = [&]() {
    for (int i = 1; i <= y; ++i) {
      if (x - i > 0 and not labo(x - i, y)) return true;
      if (y - i > 0 and not labo(x, y - i)) return true;
    }
    return false;
  }();
  memo[x][y] = win;
  return win;
}

bool solve(Int x, Int y) {
  if (x < y) swap(x, y);
  if (x == 4) {
    return y != 4;
  }
  int m = x % 4;
  if (m == 1) return y != 1;
  if (m == 2) return y != 2;
  if (m == 3) return y != 1;
  return y != 3;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
#if 1
  Int x = in, y = in;
  print(solve(x, y));
#else
  static constexpr int L = 30;
  memo.assign(L + 1, vector<optional<bool>>(L + 1));
  for (int i = 1; i <= L; ++i) {
    printf("%02d|", i);
    for (int j = 1; j <= i; ++j) {
      cout << ' ' << solve(i, j);
    }
    cout << endl;
  }
#endif
}
