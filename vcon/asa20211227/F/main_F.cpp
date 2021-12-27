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

const Real kBig = 1e50;

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  int to;
  Real cost;
};
using Graph = std::vector<std::vector<Edge>>;

struct State {
  Real cost;
  int node;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the start node to each node (if exists).
auto search_shortest_path(const Graph &g, int start, int goal) -> Real {
  const int n = g.size();
  auto mincost = vector(n, (Real) kBig);
  MinHeap<State> que;
  auto push = [&](Real cost, int node) -> bool {
    if (chmin(mincost[node], cost)) {
      que.push(State{cost, node});
      return true;
    }
    return false;
  };
  assert(push(0LL, start));

  while (not que.empty()) {
    State cur = que.top();
    que.pop();
    if (cur.cost != mincost[cur.node]) continue;
    if (cur.node == goal) break;
    for (const auto &e: g[cur.node]) {
      auto new_cost = cur.cost + e.cost;
      push(new_cost, e.to);
    }
  }
  return mincost[goal];
}

auto solve() {
  Int sx = in, sy = in, gx = in, gy = in;
  int n = in;
  vector<tuple<Int, Int, Int>> barrier(n);
  for (auto &[x, y, r]: barrier) {
    cin >> x >> y >> r;
  }
  barrier.emplace_back(sx, sy, 0);
  barrier.emplace_back(gx, gy, 0);
  const int kStart = n, kGoal = n + 1;

  Graph g(n + 2);
  REP(i, n + 2) {
    auto[xi, yi, ri] = barrier[i];
    REP(j, i + 1, n + 2) {
      auto[xj, yj, rj] = barrier[j];
      Int dx = abs(xi - xj), dy = abs(yi - yj);
      Real d = std::sqrt(dx * dx + dy * dy);
      Real rs = Real(ri) + Real(rj);
      Real cost = (d <= rs) ? 0.0 : (d - rs);
      g[i].push_back({j, cost});
      g[j].push_back({i, cost});
    }
  }

  auto res = search_shortest_path(g, kStart, kGoal);
  return res;
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
