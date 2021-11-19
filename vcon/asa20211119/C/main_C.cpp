#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
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

auto solve() -> optional<pair<vector<Int>, vector<Int>>> {
  int n = in;
  auto C = vector(n, vector(n, 0LL));
  REP(i, n) REP(j, n) cin >> C[i][j];
  auto d = vector(n - 1, 0LL);
  REP(j, 1, n) d[j - 1] = C[0][j] - C[0][j - 1];
  REP(i, 1, n) {
    auto v = vector(n - 1, 0LL);
    REP(j, 1, n) v[j - 1] = C[i][j] - C[i][j - 1];
    if (v != d) return nullopt;
  }
  vector<Int> a(n), b(n);
  a[0] = C[0][0];
  b[0] = 0;
  REP(i, n - 1) b[i + 1] = b[i] + d[i];
  REP(i, 1, n) {
    a[i] = C[i][0];
  }

  auto minb = *min_element(ALL(b));
  if (minb < 0) {
    for (auto &x: b) x -= minb;
    for (auto &x: a) x += minb;
    minb = *min_element(ALL(b));
    check(minb >= 0);
    auto mina = *min_element(ALL(a));
    if (mina < 0) return nullopt;
  }

  auto X = vector(n, vector(n, 0LL));
  REP(i, n) REP(j, n) X[i][j] = a[i] + b[j];
  check(X == C);
  return pair(a, b);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    if (not ans) {
      print("No");
    } else {
      print("Yes");
      auto[a, b] = ans.value();
      print_seq(a);
      print_seq(b);
    }
  }
}
