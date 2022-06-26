// #define NDEBUG
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
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

struct Edge {
  int to;
};
using Graph = std::vector<std::vector<Edge>>;

struct State {
  int node;
  Int cost;
};

// Returns min distance from the start node to each node (if exists).
auto search_shortest_path(const Graph &g, int start) {
  const int n = g.size() - 1;
  auto mincost = vector(n + 1, kBigVal<Int>);
  deque<State> que;
  auto push = [&](int node, Int cost) -> bool {
    if (chmin(mincost[node], cost)) {
      que.push_back(State{node, cost});
      return true;
    }
    return false;
  };
  push(start, 0LL);

  while (not que.empty()) {
    State cur = que.front();
    que.pop_front();
    if (cur.cost != mincost[cur.node]) continue;
    for (const auto &e: g[cur.node]) {
      Int new_cost = cur.cost + 1;
      push(e.to, new_cost);
    }
  }
  return mincost;
}

auto solve() {
  int n = in, m = in;
  const int kSuperNode = n;
  Graph g(n + 1);
  REP(i, m) {
    int u = in, v = in;
    --u, --v;
    if (u == -1) u = kSuperNode;
    if (v == -1) v = kSuperNode;
    if (u == kSuperNode) swap(u, v);
    g[u].push_back({v});
    if (u != kSuperNode and v != kSuperNode) {
      g[v].push_back({u});
    }
  }
  auto ds = search_shortest_path(g, 0);
  auto dg = search_shortest_path(g, n - 1);
  vector<Int> ans(n, -1);
  ans[0] = min(dg[0], dg[kSuperNode]);
  REP(i, 1, n - 1) {
    Int d = ds[n - 1];
    chmin(d, ds[i] + dg[i]);
    chmin(d, ds[i] + dg[kSuperNode]);
    chmin(d, ds[kSuperNode] + dg[i]);
    chmin(d, ds[kSuperNode] + dg[kSuperNode]);
    ans[i] = (d >= kBigVal<Int>) ? -1LL : d;
  }
  ans[n - 1] = min(ds[n - 1], ds[kSuperNode]);
  for (auto &e: ans) {
    if (e >= kBigVal<Int>) e = -1;
  }
  out_seq(ans);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
