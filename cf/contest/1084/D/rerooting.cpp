#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}

using namespace std;

struct Edge {
  int to;
  i64 len;
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
  using NodeValue = i64;
  using EdgeValue = i64;

  vector<i64> w;

  explicit Task(vector<i64> w) : w(move(w)) {}

  EdgeValue id() const { return 0LL; }

  NodeValue add_node(EdgeValue val, int node) const { return val + w[node]; }

  EdgeValue add_edge(NodeValue val, const Edge &edge) const {
    return std::max(val - edge.len, 0LL);
  }

  EdgeValue merge(const EdgeValue &x, const EdgeValue &y) const {
    return std::max(x, y);
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n;
  cin >> n;
  vector<i64> w(n);
  cin >> w;
  vector<vector<Edge>> g(n);
  REP(i, n - 1) {
    int u, v, c;
    cin >> u >> v >> c;
    --u, --v;
    g[u].push_back({v, c});
    g[v].push_back({u, c});
  }
  ReRooting<Task> rerooting(Task(move(w)), move(g));
  auto res = rerooting.run();
  cout << *max_element(ALL(res)) << "\n";
}
