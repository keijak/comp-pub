#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
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
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;
struct Edge {
  int to;
  u64 w;
};

using DP = array<Mint, 120>;

template <typename Task>
class RerootingInplace {
 private:
  using NV = typename Task::NodeValue;
  using EV = typename Task::EdgeValue;

  Task task;
  int n;                             // number of nodes
  std::vector<std::vector<Edge>> g;  // graph (tree)
  std::vector<NV> sub;               // values for each subtree rooted at i
  std::vector<NV> full;              // values for each entire tree rooted at i
  int base_root;                     // base root node where we start DFS

 public:
  explicit RerootingInplace(Task task, std::vector<std::vector<Edge>> g,
                            int r = 0)
      : task(move(task)),
        n((int)g.size()),
        g(move(g)),
        sub(n),
        full(n),
        base_root(r) {}

  const std::vector<NV> &run() {
    pull_up(base_root, -1);
    push_down(base_root, -1, task.id());
    return full;
  }

 private:
  NV pull_up(int v, int par) {
    EV res = task.id();
    for (auto &e : g[v]) {
      int u = e.to;
      if (u == par) continue;
      auto sub = task.add_edge(pull_up(u, v), e);
      task.merge_inplace(res, std::move(sub));
    }
    return (sub[v] = task.add_node(res, v));
  }

  void push_down(int v, int par, NV upper_sub) {
    int m = g[v].size();
    EV agg = task.id();

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) {
        task.merge_inplace(agg, task.add_edge(upper_sub, e));
      } else {
        task.merge_inplace(agg, task.add_edge(sub[u], e));
      }
    }
    full[v] = task.add_node(agg, v);

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) continue;
      EV edge_value = task.add_edge(sub[u], e);
      task.subtract_inplace(agg, edge_value);
      NV next_upper_sub = task.add_node(agg, v);
      push_down(u, v, std::move(next_upper_sub));
      task.merge_inplace(agg, std::move(edge_value));
    }
  }
};

// https://atcoder.jp/contests/abc201/tasks/abc201_e
struct InplaceTask {
  static constexpr int M = 60;
  using Table = std::array<Mint, M * 2>;
  using NodeValue = Table;
  using EdgeValue = Table;

  EdgeValue id() const { return Table{0}; }

  const NodeValue &add_node(const EdgeValue &val, int node) const {
    return val;
  }

  EdgeValue add_edge(const NodeValue &val, const Edge &edge) const {
    auto res = Table{};
    for (int i = 0; i < M; ++i) {
      if (edge.w >> i & 1) {
        res[2 * i] = val[i * 2 + 1];
        res[2 * i + 1] = val[i * 2] + 1;
      } else {
        res[i * 2] = val[i * 2] + 1;
        res[i * 2 + 1] = val[i * 2 + 1];
      }
    }
    return res;
  }

  void merge_inplace(EdgeValue &agg, const EdgeValue &x) const {
    for (int i = 0; i < M * 2; ++i) {
      agg[i] += x[i];
    }
  }

  void subtract_inplace(EdgeValue &agg, const EdgeValue &x) const {
    for (int i = 0; i < M * 2; ++i) {
      agg[i] -= x[i];
    }
  }
};

auto solve() {
  int n;
  cin >> n;
  vector<vector<Edge>> g(n);
  REP(i, n - 1) {
    int u, v;
    u64 w;
    cin >> u >> v >> w;
    --u, --v;
    g[u].push_back(Edge{v, w});
    g[v].push_back(Edge{u, w});
  }
  InplaceTask task;
  RerootingInplace<InplaceTask> solver(task, g);
  auto res = solver.run();
  Mint ans = 0;
  vector<Mint> pow2(60, 0);
  pow2[0] = 1;
  REP(i, 1, 60) { pow2[i] = pow2[i - 1] * 2; }
  REP(i, n) {
    const auto &dp = res[i];
    REP(j, 60) { ans += dp[j * 2 + 1] * pow2[j]; }
  }
  return ans / 2;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
