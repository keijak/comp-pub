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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template<int sign = 1>
struct Infinity {
  template<typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned_v<T>,
                  "must be positive in an unsigned type");
    if constexpr (std::numeric_limits<T>::has_infinity) {
      return T(sign) * std::numeric_limits<T>::infinity();
    } else {
      static_assert(std::numeric_limits<T>::max() != T());  // max must be defined
      return T(sign) * (std::numeric_limits<T>::max() / T(2));
    }
  }
  constexpr Infinity<sign * -1> operator-() const { return {}; }
  template<typename T>
  friend constexpr bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template<typename T>
  friend constexpr bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
constexpr Infinity<> kBig;

enum Objective {
  MINIMIZE = 1,
  MAXIMIZE = -1,
};

enum class Status {
  OPTIMAL,
  INFEASIBLE,
};

template<class Flow, class Cost, Objective objective = Objective::MINIMIZE>
class MinCostFlow {
  using V_id = uint32_t;
  using E_id = uint32_t;

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

 private:
  V_id n;
  std::vector<std::vector<Edge>> g;
  std::vector<Flow> b;

 public:
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

  EdgePtr add_edge(const V_id src, const V_id dst, const Flow lower,
                   const Flow upper, const Cost cost) {
    const E_id e = g[src].size(), re = src == dst ? e + 1 : g[dst].size();
    assert(lower <= upper);
    g[src].emplace_back(Edge{src, dst, upper, cost * objective, re});
    g[dst].emplace_back(Edge{dst, src, -lower, -cost * objective, e});
    return EdgePtr{this, src, e};
  }

  void add_supply(const V_id v, const Flow amount) { b[v] += amount; }

  void add_demand(const V_id v, const Flow amount) { b[v] -= amount; }

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
      const auto[d, u] = pq.top();
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

 public:
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
};

auto solve() {
  const i64 n = in, C = in;
  const vector<i64> a = in(n);

  MinCostFlow<int, i64> g;
  const auto kSource = g.add_vertex();
  const auto kSink = g.add_vertex();
  auto v_in = g.add_vertices(n);
  auto v_out = g.add_vertices(n);

  REP(i, n) {
    g.add_edge(kSource, v_in[i], 0, 1, 0);
    g.add_edge(v_in[i], v_out[i], 1, 1, 0);
    g.add_edge(v_out[i], kSink, 0, 1, C);
  }
  REP(i, n) {
    REP(j, i + 1, n) {
      i64 cost = abs(a[i] - a[j]);
      g.add_edge(v_out[i], v_in[j], 0, 1, cost);
    }
  }

  i64 ans = kBig;
  for (int i = 1; i <= n; ++i) {
    g.add_supply(kSource, 1);
    g.add_demand(kSink, 1);
    auto[status, mincost] = g.solve();
    if (status != Status::OPTIMAL) continue;
    DUMP(i, mincost);
    chmin(ans, mincost);
  }
  return ans;
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
