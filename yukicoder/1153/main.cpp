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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;

struct Edge {
  int to;
};

template <typename Task>
class ReRooting {
 private:
  using NV = typename Task::NodeValue;
  using EV = typename Task::EdgeValue;

  Task task;
  int n;                   // number of nodes
  vector<vector<Edge>> g;  // graph (tree)
  vector<NV> sub;          // values for each subtree rooted at i
  vector<NV> full;         // values for each entire tree rooted at i
  int base_root;           // base root node where we start DFS

 public:
  explicit ReRooting(Task task, vector<vector<Edge>> g, int r = 0)
      : task(move(task)),
        n((int)g.size()),
        g(move(g)),
        sub(n),
        full(n),
        base_root(r) {}

  const vector<NV> &run() {
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
      i64 sub = pull_up(u, v);
      res = task.merge(res, task.add_edge(sub, e));
    }
    return (sub[v] = task.add_node(res, v));
  }

  void push_down(int v, int par, NV upper_sub) {
    int m = g[v].size();
    vector<EV> cuml(m + 1, task.id()), cumr(m + 1, task.id());

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) {
        cuml[i + 1] = task.merge(cuml[i], task.add_edge(upper_sub, e));
      } else {
        cuml[i + 1] = task.merge(cuml[i], task.add_edge(sub[u], e));
      }
    }

    for (int i = m - 1; i >= 0; --i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) {
        cumr[i] = task.merge(task.add_edge(upper_sub, e), cumr[i + 1]);
      } else {
        cumr[i] = task.merge(task.add_edge(sub[u], e), cumr[i + 1]);
      }
    }

    full[v] = task.add_node(cuml[m], v);

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) continue;
      NV next_upper_sub = task.add_node(task.merge(cuml[i], cumr[i + 1]), v);
      push_down(u, v, next_upper_sub);
    }
  }
};

struct Task {
  struct NodeValue {
    int nimber;
    int to;
  };
  using EdgeValue = NodeValue;

  EdgeValue id() const { return {0, -1}; }

  NodeValue add_node(EdgeValue val, int node) const { return val; }

  EdgeValue add_edge(NodeValue val, const Edge &edge) const { return val; }

  EdgeValue merge(const EdgeValue &x, const EdgeValue &y) const {
    return std::max(x, y);
  }
};

pair<int, int> solve() {
  int n, m;
  cin >> n >> m;
  vector<int> a(n);
  REP(i, n) {
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

  Task task;
  ReRooting<Task> solver(task, g);
  auto res = solver.run();

  u64 ag = 0;
  u64 maxg = 0;
  int mi = -1, mv = -1;
  REP(i, n) {
    auto node = res[a[i]];
    ag ^= node.nimber;
    if (chmax(maxg, node.nimber)) {
      mi = i + 1;
      mv = node.next_v + 1;
    }
  }
  return {mi, mv};
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto [i, v] = solve();
  cout << i << " " << v << "\n";
}
