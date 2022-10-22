// #define NDEBUG
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
constexpr T kBig = std::numeric_limits<T>::max() / 2;
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

[[noreturn]] void exit_() {
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



// https://misawa.github.io/others/flow/library_design.html

enum Objective {
  MINIMIZE = 1,
  MAXIMIZE = -1,
};

enum class Status {
  OPTIMAL,
  INFEASIBLE,
};

using V_id = uint32_t;
using E_id = uint32_t;

template<class Flow, class Cost, Objective objective = Objective::MINIMIZE>
class MinCostFlow {
  class Edge {
    friend class MinCostFlow;

    V_id src, dst;
    Flow flow, cap;
    Cost cost;
    E_id rev;

   public:
    Edge() = default;

    Edge(const V_id src, const V_id dst, const Flow cap, const Cost cost,
         const E_id rev)
        : src(src), dst(dst), flow(0), cap(cap), cost(cost), rev(rev) {}

    [[nodiscard]] Flow residual_cap() const { return cap - flow; }
  };

  V_id n;
  std::vector<std::vector<Edge>> g;
  std::vector<Flow> b;

 public:
  class EdgePtr {
    friend class MinCostFlow;

    const MinCostFlow *instance;
    V_id v;
    E_id e;

    EdgePtr(const MinCostFlow *const instance, const V_id v, const E_id e)
        : instance(instance), v(v), e(e) {}

    [[nodiscard]] const Edge &edge() const { return instance->g[v][e]; }

    [[nodiscard]] const Edge &rev() const {
      const Edge &e = edge();
      return instance->g[e.dst][e.rev];
    }

   public:
    EdgePtr() = default;

    [[nodiscard]] V_id src() const { return v; }

    [[nodiscard]] V_id dst() const { return edge().dst; }

    [[nodiscard]] Flow flow() const { return edge().flow; }

    [[nodiscard]] Flow lower() const { return -rev().cap; }

    [[nodiscard]] Flow upper() const { return edge().cap; }

    [[nodiscard]] Cost cost() const { return edge().cost; }

    [[nodiscard]] Cost gain() const { return -edge().cost; }
  };

  MinCostFlow() : n(0) {}

  V_id add_vertex() {
    ++n;
    g.resize(n);
    b.resize(n);
    return n - 1;
  }

  std::vector<V_id> add_vertices(const size_t size) {
    std::vector<V_id> ret(size);
    std::iota(std::begin(ret), std::end(ret), n);
    n += size;
    g.resize(n);
    b.resize(n);
    return ret;
  }

  void add_supply(const V_id v, const Flow amount) { b[v] += amount; }

  void add_demand(const V_id v, const Flow amount) { b[v] -= amount; }

  // lower/upper: flow limits (inclusive, non-negative).
  // cost: cost if the objective is MINIMIZE. gain if the objective is MAXIMIZE.
  EdgePtr add_edge(const V_id src, const V_id dst, const Flow lower,
                   const Flow upper, const Cost cost) {
    const E_id e = g[src].size(), re = src == dst ? e + 1 : g[dst].size();
    assert(lower <= upper);
    g[src].emplace_back(Edge{src, dst, upper, cost * objective, re});
    g[dst].emplace_back(Edge{dst, src, -lower, -cost * objective, e});
    return EdgePtr{this, src, e};
  }

  // Solves the b flow problem.
  std::pair<Status, Cost> solve() {
    potential.resize(n);
    for (auto &es: g) {
      for (auto &e: es) {
        const Flow rcap = e.residual_cap();
        const Cost rcost = residual_cost(e.src, e.dst, e);
        if (rcost < 0 || rcap < 0) {
          push(e, rcap);
          b[e.src] -= rcap;
          b[e.dst] += rcap;
        }
      }
    }
    for (V_id v = 0; v < n; ++v) {
      if (b[v] != 0) {
        (b[v] > 0 ? excess_vs : deficit_vs).emplace_back(v);
      }
    }
    while (dual()) primal();
    Cost value = 0;
    for (const auto &es: g) {
      for (const auto &e: es) {
        value += e.flow * e.cost;
      }
    }
    value /= 2;

    if (excess_vs.empty() && deficit_vs.empty()) {
      return {Status::OPTIMAL, value / objective};
    } else {
      return {Status::INFEASIBLE, value / objective};
    }
  }

  // Solves the s-t flow problem.
  //
  // Computes the maximum flow from s to t, and also returns
  // the minimum cost when the flow is maximum.
  // Don't need to call add_supply()/add_demand() in advance.
  std::tuple<Status, Cost, Flow> solve(const V_id s, const V_id t) {
    assert(s != t);
    Flow inf_flow = std::abs(b[s]);
    for (const auto &e: g[s]) {
      inf_flow += std::max(e.cap, static_cast<Flow>(0));
    }

    add_edge(t, s, 0, inf_flow, 0);
    const auto [status, circulation_value] = solve();

    if (status == Status::INFEASIBLE) {
      g[s].pop_back();
      g[t].pop_back();
      return {status, circulation_value, 0};
    }
    inf_flow = std::abs(b[s]);
    for (const auto &e: g[s]) {
      inf_flow += e.residual_cap();
    }
    b[s] += inf_flow;
    b[t] -= inf_flow;
    const auto [mf_status, mf_value] = solve();
    b[s] -= inf_flow;
    b[t] += inf_flow;
    g[s].pop_back();
    g[t].pop_back();
    return {Status::OPTIMAL, mf_value, b[t]};
  }

 private:
  // Variables used in calculation
  static Cost constexpr unreachable = std::numeric_limits<Cost>::max();
  Cost farthest;
  std::vector<Cost> potential;
  std::vector<Cost> dist;
  std::vector<Edge *> parent;  // out-forrest.
  std::priority_queue<std::pair<Cost, int>, std::vector<std::pair<Cost, int>>,
                      std::greater<>>
      pq;  // should be empty outside of dual()
  std::vector<V_id> excess_vs, deficit_vs;

  Edge &rev(const Edge &e) { return g[e.dst][e.rev]; }

  void push(Edge &e, const Flow amount) {
    e.flow += amount;
    g[e.dst][e.rev].flow -= amount;
  }

  Cost residual_cost(const V_id src, const V_id dst, const Edge &e) {
    return e.cost + potential[src] - potential[dst];
  }

  bool dual() {
    dist.assign(n, unreachable);
    parent.assign(n, nullptr);
    excess_vs.erase(std::remove_if(std::begin(excess_vs), std::end(excess_vs),
                                   [&](const V_id v) { return b[v] <= 0; }),
                    std::end(excess_vs));
    deficit_vs.erase(
        std::remove_if(std::begin(deficit_vs), std::end(deficit_vs),
                       [&](const V_id v) { return b[v] >= 0; }),
        std::end(deficit_vs));
    for (const auto v: excess_vs) pq.emplace(dist[v] = 0, v);
    farthest = 0;
    std::size_t deficit_count = 0;
    while (!pq.empty()) {
      const auto [d, u] = pq.top();
      pq.pop();
      if (dist[u] < d) continue;
      farthest = d;
      if (b[u] < 0) ++deficit_count;
      if (deficit_count >= deficit_vs.size()) break;
      for (auto &e: g[u]) {
        if (e.residual_cap() <= 0) continue;
        const auto v = e.dst;
        const auto new_dist = d + residual_cost(u, v, e);
        if (new_dist >= dist[v]) continue;
        pq.emplace(dist[v] = new_dist, v);
        parent[v] = &e;
      }
    }
    pq = decltype(pq)();  // pq.clear() doesn't exist.
    for (V_id v = 0; v < n; ++v) {
      potential[v] += std::min(dist[v], farthest);
    }
    return deficit_count > 0;
  }

  void primal() {
    for (const auto t: deficit_vs) {
      if (dist[t] > farthest) continue;
      Flow f = -b[t];
      V_id v;
      for (v = t; parent[v] != nullptr; v = parent[v]->src) {
        f = std::min(f, parent[v]->residual_cap());
      }
      f = std::min(f, b[v]);
      if (f <= 0) continue;
      for (v = t; parent[v] != nullptr;) {
        auto &e = *parent[v];
        push(e, f);
        int u = parent[v]->src;
        if (e.residual_cap() <= 0) parent[v] = nullptr;
        v = u;
      }
      b[t] += f;
      b[v] -= f;
    }
  }
};

auto solve() {
  int H = in, W = in;
  vector<string> S(H);
  REP(i, H) cin >> S[i];

  vector<set<int>> hwall(H), vwall(W);
  REP(i, H) {
    hwall[i].insert(-1);
    hwall[i].insert(W);
  }
  REP(j, W) {
    vwall[j].insert(-1);
    vwall[j].insert(H);
  }
  REP(i, H) REP(j, W) {
      if (S[i][j] == '#') {
        hwall[i].insert(j);
        vwall[j].insert(i);
      }
    }
  MinCostFlow<int, int> mcf;
  map<pair<int, int>, int> vid, uid;
  const int kSource = mcf.add_vertex();
  const int kSink = mcf.add_vertex();

  auto add_v = [&](int x, int y) -> int {
    int id;
    if (not vid.count(pair{x, y})) {
      id = mcf.add_vertex();
      mcf.add_edge(kSource, id, 0, 1, 0);
      vid[pair{x, y}] = id;
    } else {
      id = vid[pair{x, y}];
    }
    return id;
  };
  REP(i, H) REP(j, W) {
      if (S[i][j] == '#') continue;
      int u = mcf.add_vertex();
      uid[pair{i, j}] = u;
      mcf.add_edge(u, kSink, 0, 1, 0);
      auto it = hwall[i].lower_bound(j);

      int v = add_v(i, *it);
      mcf.add_edge(v, u, 0, 1, 1);
      --it;
      v = add_v(i, *it);
      mcf.add_edge(v, u, 0, 1, 1);
      it = vwall[j].lower_bound(i);
      v = add_v(*it, j);
      mcf.add_edge(v, u, 0, 1, 1);
      --it;
      v = add_v(*it, j);
      mcf.add_edge(v, u, 0, 1, 1);
    }

  int b = ssize(uid);
  mcf.add_demand(kSink, b);
  mcf.add_supply(kSource, b);

  auto [status, cost] = mcf.solve();
  out(cost);

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
