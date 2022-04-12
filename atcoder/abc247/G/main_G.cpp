#include <bits/stdc++.h>
#include <atcoder/mincostflow>
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
void exit_() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

// Usage: Single flow call.
//
//   MinCostFlowDAG<int,int> mcf(n);
//   for (...) mcf.add_edge(...);
//   auto res = mcf.flow(kSource, kSink, flow);
//
// Usage:  Multiple flow calls.
//
//   MinCostFlowDAG<int,int> mcf(n);
//   for (...) mcf.add_edge(...);
//   for (int f = 1; f <= X; ++f) {
//     auto mcf_runner = mcf;
//     auto res = mcf_runner.flow(kSource, kSink, f);
//   }

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
  std::vector<Cost> H, dist;
  std::vector<int> indeg, prevv, preve;
  std::optional<int> source_vertex_;
  std::vector<std::pair<Cap, Cost>> last_slope_;

  explicit MinCostFlowDAG(int node_count)
      : V(node_count),
        G(V),
        H(V, INF),  // potential table
        dist(V, 0),  // distance from source_vertex_
        indeg(V, 0),  // indegree of from each vertex
        prevv(V),
        preve(V) {}

  void add_edge(const int from, const int to, const Cap cap, const Cost cost) {
    if (cap == 0) return;
    G[from].push_back(Edge{to, (int) G[to].size(), cap, cost});
    G[to].push_back(Edge{from, (int) G[from].size() - 1, 0, -cost});
    ++indeg[to];
  }

  // Calculates (max_flow, min_cost) where max_flow <= flow_limit.
  // The piece-wise linear curve of (flow, cost) history is stored in `last_slope_`.
  std::pair<Cap, Cost> flow(const int v_source,
                            const int v_sink,
                            Cap flow_limit = std::numeric_limits<Cap>::max() / 4) {
    maybe_build(v_source);  // Initialize the potential table.
    const Cap initial_flow_limit = flow_limit;
    Cost cur_cost = 0, min_cost = 0;
    Cap max_flow = 0;
    last_slope_.clear();  // Discard the previous record.
    last_slope_.emplace_back(0, 0);
    while (flow_limit > 0) {
      dijkstra(v_source);
      if (dist[v_sink] >= INF) break;
      update(v_source, v_sink, flow_limit, cur_cost);
      Cap cur_flow = initial_flow_limit - flow_limit;
      last_slope_.emplace_back(cur_flow, cur_cost);
      if (cur_flow >= max_flow) {
        max_flow = cur_flow;
        if (min_cost > cur_cost) min_cost = cur_cost;
      }
    }
    return {max_flow, min_cost};
  }

  // Cost slope for the last flow() call.
  const std::vector<std::pair<Cap, Cost>> &slope() const {
    return last_slope_;
  }

 private:
  struct DijkstraState {
    Cost dist;
    int node;
  };
  friend bool operator>(const DijkstraState &x, const DijkstraState &y) {
    return x.dist > y.dist;
  }

  // Calculate the potential for the given source node.
  void maybe_build(const int v_source) {
    if (source_vertex_.has_value()) {
      assert(source_vertex_.value() == v_source);
      return;
    }
    source_vertex_ = v_source;
    auto ord = topological_sort();
    assert(ord.has_value());
    calc_potential(*ord, v_source);
  }

  std::optional<std::vector<int>> topological_sort() {
    std::vector<int> ord;
    std::queue<int> que;
    for (int i = 0; i < V; ++i) {
      if (indeg[i] == 0) que.push(i);
    }
    while (!que.empty()) {
      const int p = que.front();
      que.pop();
      ord.push_back(p);
      for (auto &e: G[p]) {
        if (e.cap > 0 && --indeg[e.to] == 0) que.push(e.to);
      }
    }
    if (*max_element(indeg.begin(), indeg.end()) != 0) {
      return std::nullopt;
    }
    return ord;
  }

  void calc_potential(const std::vector<int> &ord, const int s) {
    H[s] = 0;
    for (const int v: ord) {
      if (H[v] == INF) continue;
      for (const Edge &e: G[v]) {
        if (e.cap > 0) H[e.to] = min(H[e.to], H[v] + e.cost);
      }
    }
  }

  void dijkstra(const int s) {
    using Heap = std::priority_queue<
        DijkstraState, std::vector<DijkstraState>, std::greater<>>;
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
        if (e.cap > 0 and dist[e.to] > dist[v] + e.cost + H[v] - H[e.to]) {
          dist[e.to] = dist[v] + e.cost + H[v] - H[e.to];
          prevv[e.to] = v;
          preve[e.to] = i;
          heap.push(DijkstraState{dist[e.to], e.to});
        }
      }
    }
  }

  void update(const int s, const int t, Cap &f, Cost &res) {
    for (int i = 0; i < V; i++) {
      if (dist[i] != INF) H[i] += dist[i];
    }
    Cap d = f;
    for (int v = t; v != s; v = prevv[v]) {
      d = min(d, G[prevv[v]][preve[v]].cap);
    }
    f -= d;
    res += H[t] * d;
    for (int v = t; v != s; v = prevv[v]) {
      Edge &e = G[prevv[v]][preve[v]];
      e.cap -= d;
      G[v][e.rev].cap += d;
    }
  }
};

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(std::move(v)) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }
};

auto solve() {
  const int n = in;
  vector<tuple<int, int, Int>> edges(n);
  map<pair<int, int>, Int> edge_gain;
  vector<int> va, vb;

  REP(i, n) {
    const int a = int(in) - 1;
    const int b = int(in) - 1;
    const Int c = in;
    va.push_back(a);
    vb.push_back(b);
    edges[i] = make_tuple(a, b, c);
    if (edge_gain.count({a, b})) {
      chmax(edge_gain[{a, b}], c);
    } else {
      edge_gain[{a, b}] = c;
    }
  }
  Compressed<int> ca(va), cb(vb);

  const int V = ca.size() + cb.size() + 2;
  const int kSource = ca.size() + cb.size();
  const int kSink = kSource + 1;

  MinCostFlowDAG<int, Int> mcf(V);
  REP(v, ca.size()) {
    mcf.add_edge(kSource, v, 1, 0);
  }
  REP(v, cb.size()) {
    mcf.add_edge(v + ca.size(), kSink, 1, 0);
  }
  for (auto[fromto, gain]: edge_gain) {
    auto[a, b] = fromto;
    int ai = ca.index(a);
    int bi = cb.index(b);
    mcf.add_edge(ai, ca.size() + bi, 1, -gain);
  }
  mcf.flow(kSource, kSink);
  const auto &slope = mcf.last_slope_;

  vector<Int> ans;
  for (int i = 1; i < ssize(slope); ++i) {
    const auto&[x1, y1] = slope[i - 1];
    const auto&[x2, y2] = slope[i];
    const Int z1 = -y1;
    const Int z2 = -y2;
    const Int d = (z2 - z1) / (x2 - x1);
    Int gain = z1;
    for (Int k = x1 + 1; k <= x2; ++k) {
      gain += d;
      ans.push_back(gain);
    }
  }
  print(ssize(ans));
  print_seq(ans, "\n");
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}
