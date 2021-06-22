#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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

void print() { std::cout << "\n"; }
template <class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template <typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template <typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define VAR(type, ...) \
  type __VA_ARGS__;    \
  read_from_cin(__VA_ARGS__);

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// Most Significant Set Bit (Highest One Bit) = std::bit_floor(x)
int mssb_pos(unsigned x) {
  static const int CLZ_WIDTH = __builtin_clz(1);
  assert(x != 0);
  return CLZ_WIDTH - __builtin_clz(x);
}
int mssb_pos(u64 x) {
  static const int CLZLL_WIDTH = __builtin_clzll(1LL);
  assert(x != 0);
  return CLZLL_WIDTH - __builtin_clzll(x);
}
template <typename U>
inline U bit_floor(U x) {
  if (x == 0) return 0;
  return U(1) << mssb_pos(x);
}

struct Edge {
  int to;
};

template <typename Task>
class InplaceRerooting {
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
  explicit InplaceRerooting(Task task, std::vector<std::vector<Edge>> g,
                            int r = 0)
      : task(move(task)),
        n((int)g.size()),
        g(move(g)),
        sub(n),
        full(n),
        base_root(r) {}

  const std::vector<NV> &run() {
    pull_up(base_root, -1);
    push_down(base_root, -1, std::nullopt);
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

  void push_down(int v, int par, std::optional<NV> upper_sub) {
    int m = g[v].size();
    EV agg = task.id();

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) {
        assert(upper_sub.has_value());
        task.merge_inplace(agg, task.add_edge(std::move(*upper_sub), e));
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
      push_down(u, v, std::optional<NV>{task.add_node(agg, v)});
      task.merge_inplace(agg, std::move(edge_value));
    }
  }
};

struct InplaceTask {
  struct NodeValue {
    unsigned nimber;
    map<unsigned, set<int>> nexts;
  };
  struct EdgeValue {
    map<unsigned, set<int>> nimbers;
  };

  EdgeValue id() const { return {}; }

  NodeValue add_node(const EdgeValue &e, int node) const {
    NodeValue res;
    unsigned mex = 0;
    for (auto &[g, tos] : e.nimbers) {
      if (g == mex) {
        auto &nex = res.nexts[g];
        for (auto &to : tos) {
          if (ssize(nex) >= 2) break;
          nex.insert(to);
        }
        mex++;
      } else if (g > mex) {
        break;
      }
    }
    res.nimber = mex;
    return res;
  }

  EdgeValue add_edge(const NodeValue &val, const Edge &edge) const {
    EdgeValue res;
    res.nimbers[val.nimber].insert(edge.to);
    return res;
  }

  void merge_inplace(EdgeValue &agg, const EdgeValue &x) const {
    for (auto &[g, tos] : x.nimbers) {
      agg.nimbers[g].insert(ALL(tos));
      auto &nex = agg.nimbers[g];
      for (auto &to : tos) {
        if (ssize(nex) >= 2) break;
        nex.insert(to);
      }
    }
  }

  void subtract_inplace(EdgeValue &agg, const EdgeValue &x) const {
    for (auto &[g, tos] : x.nimbers) {
      auto &nimg = agg.nimbers[g];
      for (auto to : tos) {
        nimg.erase(to);
        if (nimg.empty()) {
          agg.nimbers.erase(g);
        }
      }
    }
  }
};

pair<int, int> solve() {
  int n, m;
  cin >> n >> m;
  vector<int> a(m);
  REP(i, m) {
    cin >> a[i];
    --a[i];
  }
  vector<vector<Edge>> g(n);
  REP(i, n - 1) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    g[u].push_back({v});
    g[v].push_back({u});
  }

  InplaceTask task;
  InplaceRerooting<InplaceTask> solver(task, g);
  auto res = solver.run();

  unsigned ag = 0;
  REP(i, m) {
    const auto &node = res[a[i]];
    ag ^= node.nimber;
  }
  if (ag == 0) {
    return {-1, -1};
  }

  unsigned ag_mssb = bit_floor(ag);
  int mi = -1;
  REP(i, m) {
    const auto &node = res[a[i]];
    if (node.nimber & ag_mssb) {
      mi = i;
      break;
    }
  }
  DUMP(mi);
  assert(mi >= 0);
  const auto &nv = res[a.at(mi)];
  unsigned tg = (nv.nimber ^ ag);
  auto it = nv.nexts.find(tg);
  assert(it != nv.nexts.end());
  assert(not it->second.empty());
  int to = *it->second.begin();
  return {mi + 1, to + 1};
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto [i, v] = solve();
  print(i, v);
}
