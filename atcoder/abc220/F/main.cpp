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

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;


struct Edge {
  int to;
};

template<typename Task>
class InplaceRerooting {
 private:
  using NV = typename Task::NodeValue;
  using EV = typename Task::EdgeValue;

  Task task_;
  int n_;                             // number of nodes
  std::vector<std::vector<Edge>> g_;  // graph (tree)
  std::vector<NV> sub_result_;               // values for each subtree rooted at i
  std::vector<NV> full_result_;              // values for each entire tree rooted at i
  int base_root_;                     // base root node where we start DFS

 public:
  explicit InplaceRerooting(Task task, std::vector<std::vector<Edge>> g,
                            int r = 0)
      : task_(move(task)),
        n_((int) g.size()),
        g_(move(g)),
        sub_result_(n_),
        full_result_(n_),
        base_root_(r) {}

  const std::vector<NV> &run() {
    pull_up(base_root_, -1);
    push_down(base_root_, -1, std::nullopt);
    return full_result_;
  }

 private:
  NV pull_up(int v, int par) {
    EV res = task_.id();
    for (auto &e: g_[v]) {
      int u = e.to;
      if (u == par) continue;
      auto sub = task_.add_edge(pull_up(u, v), e);
      task_.merge_inplace(res, std::move(sub));
    }
    return (sub_result_[v] = task_.add_node(res, v));
  }

  void push_down(int v, int par, std::optional<NV> upper_sub) {
    int m = g_[v].size();
    EV agg = task_.id();

    for (int i = 0; i < m; ++i) {
      auto &e = g_[v][i];
      int u = e.to;
      if (u == par) {
        assert(upper_sub.has_value());
        task_.merge_inplace(agg, task_.add_edge(std::move(*upper_sub), e));
      } else {
        task_.merge_inplace(agg, task_.add_edge(sub_result_[u], e));
      }
    }
    full_result_[v] = task_.add_node(agg, v);

    for (int i = 0; i < m; ++i) {
      auto &e = g_[v][i];
      int u = e.to;
      if (u == par) continue;
      EV edge_value = task_.add_edge(sub_result_[u], e);
      task_.subtract_inplace(agg, edge_value);
      std::optional<NV> next_upper_sub{task_.add_node(agg, v)};
      push_down(u, v, std::move(next_upper_sub));
      task_.merge_inplace(agg, std::move(edge_value));
    }
  }
};

struct InplaceTask {
  struct T {
    i64 node_count;
    i64 edge_count;
    T(i64 nc = 0, i64 ec = 0) : node_count(nc), edge_count(ec) {}
  };
  using NodeValue = T;
  using EdgeValue = T;

  EdgeValue id() const { return {0, 0}; }

  NodeValue add_node(EdgeValue val, int node) const {
    val.node_count += 1;
    return val;
  }

  EdgeValue add_edge(const NodeValue &val, const Edge &edge) const {
    EdgeValue res = val;
    res.edge_count += val.node_count;
    return res;
  }

  void merge_inplace(EdgeValue &agg, const EdgeValue &x) const {
    agg.node_count += x.node_count;
    agg.edge_count += x.edge_count;
  }

  void subtract_inplace(EdgeValue &agg, const EdgeValue &x) const {
    agg.node_count -= x.node_count;
    agg.edge_count -= x.edge_count;
  }
};

auto solve() {
  const int n = in;
  vector<vector<Edge>> g(n);
  REP(i, n - 1) {
    int u = in, v = in;
    --u, --v;
    g[u].push_back({v});
    g[v].push_back({u});
  }
  InplaceRerooting<InplaceTask> r(InplaceTask{}, g);
  auto res = r.run();
  REP(v, n) {
    auto x = res[v];
    DUMP(n, x.node_count, x.edge_count);
    print(x.edge_count);
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
