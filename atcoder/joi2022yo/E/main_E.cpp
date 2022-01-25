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

struct UndoableUnionFind {
  std::vector<int> parent_;
  std::stack<std::pair<int, int>> history_;

  explicit UndoableUnionFind(int sz) { parent_.assign(sz, -1); }

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    history_.emplace(x, parent_[x]);
    history_.emplace(y, parent_[y]);
    if (x == y) return (false);
    if (parent_[x] > parent_[y]) std::swap(x, y);
    parent_[x] += parent_[y];
    parent_[y] = x;
    return (true);
  }

  int find(int k) {
    if (parent_[k] < 0) return (k);
    return (find(parent_[k]));
  }

  int size(int k) { return (-parent_[find(k)]); }

  bool same(int x, int y) { return find(x) == find(y); }

  void undo() {
    parent_[history_.top().first] = history_.top().second;
    history_.pop();
    parent_[history_.top().first] = history_.top().second;
    history_.pop();
  }

  void snapshot() {
    while (history_.size()) history_.pop();
  }

  void rollback() {
    while (history_.size()) undo();
  }
};

struct Edge {
  int u, v;
};

struct splitmix64 {
  // http://xorshift.di.unimi.it/splitmix64.c
  size_t operator()(std::uint64_t x) const {
    static const std::uint64_t FIXED_RANDOM = std::random_device{}();
    x += FIXED_RANDOM;
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int n = in, m = in, K = in;
  UndoableUnionFind uf(n);
  vector<Edge> edges(m);
  REP(i, m) {
    int u = in, v = in;
    --u, --v;
    edges[i] = {u, v};
  }
  vector<int> states = in(n);
  for (auto &x: states) --x;

  unordered_map<Uint, vector<int>, splitmix64> edgemap;
  edgemap.reserve(1 << 20);
  edgemap.max_load_factor(0.25);

  REP(i, m) {
    auto e = edges[i];
    if (states[e.u] == states[e.v]) {
      uf.unite(e.u, e.v);
    }
    array<int, 2> s1 = {states[e.u], states[e.v]};
    if (s1[0] > s1[1]) swap(s1[0], s1[1]);
    Uint key = (Uint(s1[0]) << 32) | s1[1];
    edgemap[key].push_back(i);
  }
  uf.snapshot();

  const int Q = in;
  vector<int> ans(Q, -1);
  vector<pair<int, int>> queries(Q);
  unordered_map<Uint, vector<int>, splitmix64> querymap;
  querymap.reserve(1 << 20);
  querymap.max_load_factor(0.25);

  REP(qi, Q) {
    int a = in, b = in;
    --a, --b;
    queries[qi] = {a, b};
    if (states[a] == states[b]) {
      ans[qi] = uf.same(a, b) ? 1 : 0;
      continue;
    }
    array<int, 2> sq = {states[a], states[b]};
    if (sq[0] > sq[1]) swap(sq[0], sq[1]);
    Uint key = (Uint(sq[0]) << 32) | sq[1];
    querymap[key].push_back(qi);
  }
  for (const auto&[key, es]: edgemap) {
    for (auto i: es) {
      const auto &e = edges[i];
      uf.unite(e.u, e.v);
    }
    for (auto qi: querymap[key]) {
      const auto&[a, b] = queries[qi];
      ans[qi] = uf.same(a, b) ? 1 : 0;
    }
    uf.rollback();
  }
  REP(i, Q) {
    if (ans[i] == -1) ans[i] = 0; // no edge
  }
  print_seq(ans, "\n");
}
