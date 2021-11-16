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

struct Void {};  // No print.

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, Void>) {
    return std::cout;  // Nothing.
  } else if constexpr (std::is_same_v<T, bool>) {
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

auto solve() -> vector<int> {
  int n = in;
  vector<vector<Int>> a(n);
  REP(i, n) {
    int ci = in;
    a[i].resize(ci);
    REP(j, ci) { cin >> a[i][j]; }
  }
  int m = in;
  set<vector<int>> banned;
  REP(i, m) {
    vector<int> b(n);
    REP(j, n) {
      cin >> b[j];
      b[j]--;
    }
    banned.insert(b);
  }
  priority_queue<pair<Int, vector<int>>> pq;
  set<vector<int>> visited;
  {
    Int s = 0;
    vector<int> bx(n);
    REP(i, n) {
      int j = ssize(a[i]) - 1;
      bx[i] = j;
      s += a[i][j];
    }
    pq.emplace(s, bx);
    visited.emplace(bx);
  }
  while (not pq.empty()) {
    auto[s, b] = pq.top();
    pq.pop();
    if (not banned.count(b)) {
      return b;
    }
    REP(i, n) {
      if (b[i] > 0) {
        int j = --b[i];
        if (visited.insert(b).second) {
          Int s2 = s - (a[i][j + 1] - a[i][j]);
          pq.emplace(s2, b);
        }
        ++b[i];
      }
    }
  }
  check(false);
  return {};
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    for (auto &e: ans) ++e;
    print_seq(ans);
  }
}
