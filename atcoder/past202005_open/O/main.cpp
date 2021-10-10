#include <bits/stdc++.h>
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
  return (int) a.size();
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
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
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct VersatileInput {
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
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#define cerr if(false)std::cerr
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
  std::vector<std::vector<Edge>> G;
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
    std::queue<int> que;
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
        const Edge &e = G[v][i];
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
  int n = in, m = in;
  vector<i64> A = in(n);
  vector<i64> B = in(n);
  vector<int> R = in(3);
  MinCostFlowDAG<int, i64> mcf(n + 3 + 2);
  auto round_node = [&](int i) { return n + i; };
  const int kSource = n + 3;
  const int kSink = n + 4;
  REP(i, 3) {
    int r = round_node(i);
    mcf.add_edge(kSource, r, m, 0);
    REP(j, n) {
      i64 co = A[j];
      REP(k, i + 1) co *= B[j];
      mcf.add_edge(r, j, 1, -(co % R[i]));
    }
  }
  REP(j, n) {
    i64 p1 = A[j] * B[j];
    i64 p2 = A[j] * B[j] * (B[j] - 1);
    i64 p3 = A[j] * B[j] * B[j] * (B[j] - 1);
    mcf.add_edge(j, kSink, 1, p1);
    mcf.add_edge(j, kSink, 1, p2);
    mcf.add_edge(j, kSink, 1, p3);
  }
  auto res = mcf.flow(kSource, kSink, 3 * m);
  check(res.has_value());
  return -res.value();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
