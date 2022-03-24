#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = b), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = b), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)a.size();
}
template <typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

template <typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template <typename T>
inline std::ostream &print(const T &x) {
  return print_one(x, '\n');
}
template <typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template <typename Container>
std::ostream &print_seq(const Container &seq, const char *sep = " ",
                        const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template <typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template <typename T>
    operator T() const {
      T xs(n);
      for (auto &x : xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "backward.hpp"
#include "debug_dump.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr \
  if (false) cerr
#endif

using namespace std;

vector<vector<optional<int>>> memo;

bool solve(int a, int b) {
  if (a < b) swap(a, b);
  if (memo[a][b].has_value()) {
    return (bool)memo[a][b].value();
  }
  if (a == 0 and b == 0) {
    return false;
  }
  if (b == 0) {
    return true;
  }
  bool win = false;

  // Choose A.
  for (int i = a; i >= 0; --i) {
    int j = a - i;
    if (j > b) break;
    if (not solve(a - i, b - j)) {
      win = true;
      break;
    }
  }
  if (not win) {
    // Choose B.
    for (int i = b; i >= 0; --i) {
      int j = b - i;
      if (not solve(a - i, b - j)) {
        win = true;
        break;
      }
    }
  }
  memo[a][b] = win;
  return win;
}

int main() {
  memo.assign(20, vector<optional<int>>(20));
  for (int a = 1; a < 20; ++a) {
    vector<int> v(a);
    for (int b = 1; b <= a; ++b) {
      bool win = solve(a, b);
      v[b - 1] = win;
    }
    cout << "# a=" << a << ": ";
    print_seq(v);
    cout.flush();
  }
}
