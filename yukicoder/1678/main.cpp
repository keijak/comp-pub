#include <bits/stdc++.h>
#include <atcoder/mincostflow>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x: a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template<typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}
template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template<typename Cap, typename Cost>
class MinCostFlowDAG {
 public:
  static constexpr Cost INF = std::numeric_limits<Cost>::max();

  struct Edge {
    int to, rev;
    Cap cap;
    Cost cost;
  };

  const int V;
  std::vector<vector<Edge>> G;
  std::vector<Cost> h, dist;
  std::vector<int> deg, ord, prevv, preve;

  explicit MinCostFlowDAG(int node_count)
      : V(node_count),
        G(V),
        h(V, INF),
        dist(V, 0),
        deg(V, 0),
        prevv(V),
        preve(V) {}

  void add_edge(const int from, const int to, const Cap cap, const Cost cost) {
    if (cap == 0) return;
    G[from].push_back(Edge{to, (int) G[to].size(), cap, cost});
    G[to].push_back(Edge{from, (int) G[from].size() - 1, 0, -cost});
    ++deg[to];
  }

  std::optional<Cost> flow(const int s, const int t, Cap f) {
    assert(topological_sort());  // must be a DAG.
    calc_potential(s);
    Cost res = 0;
    while (f > 0) {
      dijkstra(s);
      if (dist[t] == INF) return std::nullopt;
      update(s, t, f, res);
    }
    return res;
  }

 private:
  struct DijkstraState {
    Cost dist;
    int node;
  };
  friend bool operator>(const DijkstraState &x, const DijkstraState &y) {
    return x.dist > y.dist;
  }

  bool topological_sort() {
    queue<int> que;
    for (int i = 0; i < V; ++i) {
      if (deg[i] == 0) que.push(i);
    }
    while (!que.empty()) {
      const int p = que.front();
      que.pop();
      ord.push_back(p);
      for (auto &e: G[p]) {
        if (e.cap > 0 && --deg[e.to] == 0) que.push(e.to);
      }
    }
    return (*max_element(deg.begin(), deg.end()) == 0);
  }

  void calc_potential(const int s) {
    h[s] = 0;
    for (const int v: ord) {
      if (h[v] == INF) continue;
      for (const Edge &e: G[v]) {
        if (e.cap > 0) h[e.to] = min(h[e.to], h[v] + e.cost);
      }
    }
  }

  void dijkstra(const int s) {
    using Heap = std::priority_queue<DijkstraState, std::vector<DijkstraState>,
                                     std::greater<DijkstraState>>;
    Heap heap;
    fill(dist.begin(), dist.end(), INF);
    dist[s] = 0;
    heap.push(DijkstraState{0, s});
    while (not heap.empty()) {
      const auto cur = heap.top();
      heap.pop();
      const int v = cur.node;
      if (dist[v] < cur.dist) continue;
      for (int i = 0; i < (int) G[v].size(); ++i) {
        Edge &e = G[v][i];
        if (e.cap > 0 and dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
          dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
          prevv[e.to] = v;
          preve[e.to] = i;
          heap.push(DijkstraState{dist[e.to], e.to});
        }
      }
    }
  }

  void update(const int s, const int t, Cap &f, Cost &res) {
    for (int i = 0; i < V; i++) {
      if (dist[i] != INF) h[i] += dist[i];
    }
    Cap d = f;
    for (int v = t; v != s; v = prevv[v]) {
      d = min(d, G[prevv[v]][preve[v]].cap);
    }
    f -= d;
    res += h[t] * d;
    for (int v = t; v != s; v = prevv[v]) {
      Edge &e = G[prevv[v]][preve[v]];
      e.cap -= d;
      G[v][e.rev].cap += d;
    }
  }
};

auto solve() {
  const int n = in, K = in;
  vector<i64> a(n);
  MinCostFlowDAG<int, i64> g(n);
  REP(i, n) {
    a[i] = in;
    int m = in;
    REP(j, m) {
      int b = in;
      --b;
      i64 cost = a[b] - a[i];
      if (cost < 0) {
        g.add_edge(b, i, 1, cost);
      }
    }
  }
  REP(i, n - 1) {
    g.add_edge(i, i + 1, K, 0);
  }
  auto ans = g.flow(0, n - 1, K);
  assert(ans.has_value());
  return -ans.value();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto ans = solve();
  print(ans);
}
