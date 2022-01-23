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

template<class T>
using MinHeap = std::priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  int to;
  Int cost;
  bool rev;
};
using Graph = std::vector<std::vector<Edge>>;

struct State {
  Int cost;
  int rcnt;
  int node;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the start node to each node (if exists).
auto search_shortest_path(const Graph &g, int start, int goal, Int L, int M) {
  const int n = g.size();
  auto mincost = vector(n, vector(M + 1, kBigVal<Int>));
  MinHeap<State> que;
  auto push = [&](Int cost, int rcnt, int node) -> bool {
    if (chmin(mincost[node][rcnt], cost)) {
      que.push(State{cost, rcnt, node});
      return true;
    }
    return false;
  };
  assert(push(0LL, start, 0));

  while (not que.empty()) {
    State cur = que.top();
    que.pop();
    if (cur.cost != mincost[cur.node][cur.rcnt]) continue;
    for (const auto &e: g[cur.node]) {
      auto new_cost = cur.cost + e.cost;
      if (new_cost > L) continue;
      int new_rcnt = cur.rcnt;
      if (e.rev) new_rcnt++;
      if (new_rcnt > M) continue;
      push(new_cost, new_rcnt, e.to);
    }
  }
  return mincost[goal];
}

auto solve() -> int {
  int n = in, m = in;
  Int L = in;
  Graph g(n);
  REP(i, m) {
    int a = in, b = in;
    Int c = in;
    --a, --b;
    g[a].push_back({b, c, false});
    g[b].push_back({a, c, true});
  }
  auto mc = search_shortest_path(g, 0, n - 1, L, m);
  REP(i, m + 1) {
    if (mc[i] <= L) return i;
  }
  return -1;
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
