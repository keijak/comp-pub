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

struct UnionFind {
  int n;
  mutable std::vector<int> parent;  // positive: parent, negative: size
  std::set<int> roots_;

  explicit UnionFind(int sz) : n(sz), parent(sz, -1) {
    for (int i = 0; i < sz; ++i) roots_.insert(i);
  }

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent[x] < -parent[y]) std::swap(x, y);
    parent[x] += parent[y];
    parent[y] = x;
    roots_.erase(y);
    return true;
  }

  int find(int v) const {
    if (parent[v] < 0) return v;
    return parent[v] = find(parent[v]);
  }

  int size(int v) const { return -parent[find(v)]; }

  bool same(int x, int y) const { return find(x) == find(y); }

  const std::set<int> &roots() const { return roots_; }
};

auto solve() {
  int n = in, m = in;
  vector<set<int>> g(n);
  UnionFind uf(n);
  REP(i, m) {
    int a = in, b = in;
    --a, --b;
    g[a].insert(b);
    g[b].insert(a);
    if (not uf.unite(a, b)) return false;
  }
  REP(v, n) {
    if (ssize(g[v]) > 2) return false;
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
