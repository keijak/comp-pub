#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
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
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
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

Int solve(const string &S, int K) {
  int n = ssize(S);
  Int d = 0, m = 0, c = 0, dm = 0, mc = 0, dmc = 0;
  REP(i, n) {
    int l = i - K;
    if (l >= 0) {
      if (S[l] == 'C') {
        c -= 1;
      }
      if (S[l] == 'M') {
        m -= 1;
        mc -= c;
      }
      if (S[l] == 'D') {
        d -= 1;
        dm -= m;
      }
    }
    if (S[i] == 'C') {
      dmc += dm;
      mc += m;
      c += 1;
    }
    if (S[i] == 'M') {
      dm += d;
      m += 1;
    }
    if (S[i] == 'D') {
      d += 1;
    }
  }
  return dmc;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int n = in;
  string S = in;
  assert(n == ssize(S));
  int Q = in;
  vector<int> K = in(Q);
  REP(i, Q) {
    print(solve(S, K[i]));
  }
}
