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

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
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
  const int n = in, m = in;
  const string s = in;  // A or B
  vector<set<int>> toA(n), toB(n);
  REP(i, m) {
    int u = in, v = in;
    --u, --v;
    (s[v] == 'A' ? toA[u] : toB[u]).insert(v);
    (s[u] == 'A' ? toA[v] : toB[v]).insert(u);
  }
  vector<int> is_good(n, 1);
  queue<int> q;
  REP(v, n) {
    if (toA[v].empty() or toB[v].empty()) {
      is_good[v] = false;
      q.push(v);
    }
  }
  while (not q.empty()) {
    auto v = q.front();
    q.pop();
    auto &to = (s[v] == 'A' ? toA : toB);
    to[v].erase(v);
    for (auto u: toA[v]) {
      to[u].erase(v);
      if (to[u].empty() and is_good[u]) {
        is_good[u] = false;
        q.push(u);
      }
    }
    for (auto u: toB[v]) {
      to[u].erase(v);
      if (to[u].empty() and is_good[u]) {
        is_good[u] = false;
        q.push(u);
      }
    }
    toA[v].clear();
    toB[v].clear();
  }
  DUMP(is_good);
  return accumulate(ALL(is_good), 0) > 0;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
