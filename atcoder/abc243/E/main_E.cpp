#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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

auto solve() {
  int n = in, m = in;
  vector<tuple<int, int, Int>> edges(m);
  auto d = vector(n, vector(n, kBigVal<Int>));
  REP(i, n) d[i][i] = 0;
  REP(i, m) {
    int a = in, b = in;
    Int c = in;
    --a, --b;
    edges[i] = {a, b, c};
    d[a][b] = d[b][a] = c;
  }

  auto ways = vector(n, vector(n, 0));

  REP(k, n) REP(i, n) REP(j, n) {
        if (d[i][k] == kBigVal<Int>) continue;
        if (d[k][j] == kBigVal<Int>) continue;
        Int co = d[i][k] + d[k][j];
        if (d[i][j] > co) {
          d[i][j] = co;
          if (k != i and k != j) ways[i][j] = 2;
        } else if (d[i][j] == co) {
          if (k != i and k != j) ways[i][j] = 2;
        }
      }

  int cnt = 0;
  REP(i, m) {
    auto[a, b, c] = edges[i];
    if (c > d[a][b]) {
      ++cnt;
    } else if (c == d[a][b] and ways[a][b] == 2) {
      DUMP(i, a + 1, b + 1, c);
      ++cnt;
    }
  }

  return cnt;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    print(solve());
  }
}
