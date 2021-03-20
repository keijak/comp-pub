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
  using T = typename Task::T;

 public:
  int n;                   // number of nodes
  vector<vector<Edge>> g;  // graph (tree)
  vector<T> sub;           // values for each subtree rooted at i
  vector<T> full;          // values for each entire tree rooted at i
  int base_root;           // base root node where we start DFS

  explicit ReRooting(vector<vector<Edge>> g, int r = 0)
      : n((int)g.size()), g(move(g)), sub(n), full(n), base_root(r) {}

  const vector<T> &run() {
    pull_up(base_root, -1);
    push_down(base_root, -1, Task::id());
    return full;
  }

 private:
  T pull_up(int v, int par) {
    T res = Task::id();
    for (auto &e : g[v]) {
      int u = e.to;
      if (u == par) continue;
      i64 sub = pull_up(u, v);
      res = Task::op(res, Task::add_edge(sub, e));
    }
    return (sub[v] = Task::add_node(res, v));
  }

  void push_down(int v, int par, T upper_sub) {
    int m = g[v].size();
    vector<T> cuml(m + 1, Task::id()), cumr(m + 1, Task::id());

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) {
        cuml[i + 1] = Task::op(cuml[i], Task::add_edge(upper_sub, e));
      } else {
        cuml[i + 1] = Task::op(cuml[i], Task::add_edge(sub[u], e));
      }
    }

    for (int i = m - 1; i >= 0; --i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) {
        cumr[i] = Task::op(Task::add_edge(upper_sub, e), cumr[i + 1]);
      } else {
        cumr[i] = Task::op(Task::add_edge(sub[u], e), cumr[i + 1]);
      }
    }

    full[v] = Task::add_node(cuml[m], v);

    for (int i = 0; i < m; ++i) {
      auto &e = g[v][i];
      int u = e.to;
      if (u == par) continue;
      T next_upper_sub = Task::add_node(Task::op(cuml[i], cumr[i + 1]), v);
      push_down(u, v, next_upper_sub);
    }
  }
};

vector<i64> w;

struct Task {
  using T = i64;

  static T id() { return 0LL; }

  static T op(const T &x, const T &y) { return std::max(x, y); }

  static T add_node(T val, int node) { return val + w[node]; }

  static T add_edge(T val, const Edge &edge) {
    return std::max(val - edge.len, 0LL);
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n;
  cin >> n;
  w.resize(n);
  cin >> w;
  vector<vector<Edge>> g(n);
  REP(i, n - 1) {
    int u, v, c;
    cin >> u >> v >> c;
    --u, --v;
    g[u].push_back({v, c});
    g[v].push_back({u, c});
  }
  ReRooting<Task> rerooting(g);
  auto res = rerooting.run();
  cout << *max_element(ALL(res)) << "\n";
}
