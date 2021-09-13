#include <bits/stdc++.h>
#include <atcoder/mincostflow>
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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x: a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
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
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template<typename CapType, typename CostType>
class MinCostFlowDAG {
 public:
  using Cat = CapType;
  using Cot = CostType;
  using pti = pair<Cot, int>;
  struct edge {
    int to, rev;
    Cat cap;
    Cot cost;
  };
  const int V;
  const Cot inf;
  vector<vector<edge> > G;
  vector<Cot> h, dist;
  vector<int> deg, ord, prevv, preve;
  MinCostFlowDAG(const int node_size) : V(node_size), inf(numeric_limits<Cot>::max()),
                                        G(V), h(V, inf), dist(V), deg(V, 0), prevv(V), preve(V) {}
  void add_edge(const int from, const int to, const Cat cap, const Cot cost) {
    if (cap == 0) return;
    G[from].push_back((edge) {to, (int) G[to].size(), cap, cost});
    G[to].push_back((edge) {from, (int) G[from].size() - 1, 0, -cost});
    ++deg[to];
  }
  bool tsort() {
    queue<int> que;
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
      if (h[v] == inf) continue;
      for (const edge &e: G[v]) {
        if (e.cap > 0) h[e.to] = min(h[e.to], h[v] + e.cost);
      }
    }
  }
  void Dijkstra(const int s) {
    priority_queue<pti, vector<pti>, greater<pti> > que;
    fill(dist.begin(), dist.end(), inf);
    dist[s] = 0;
    que.push(pti(0, s));
    while (!que.empty()) {
      pti p = que.top();
      que.pop();
      const int v = p.second;
      if (dist[v] < p.first) continue;
      for (int i = 0; i < (int) G[v].size(); ++i) {
        edge &e = G[v][i];
        if (e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
          dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
          prevv[e.to] = v, preve[e.to] = i;
          que.push(pti(dist[e.to], e.to));
        }
      }
    }
  }
  void update(const int s, const int t, Cat &f, Cot &res) {
    for (int i = 0; i < V; i++) {
      if (dist[i] != inf) h[i] += dist[i];
    }
    Cat d = f;
    for (int v = t; v != s; v = prevv[v]) {
      d = min(d, G[prevv[v]][preve[v]].cap);
    }
    f -= d;
    res += h[t] * d;
    for (int v = t; v != s; v = prevv[v]) {
      edge &e = G[prevv[v]][preve[v]];
      e.cap -= d;
      G[v][e.rev].cap += d;
    }
  }
  Cot solve(const int s, const int t, Cat f) {
    if (!tsort()) assert(false); // not DAG
    calc_potential(s);
    Cot res = 0;
    while (f > 0) {
      Dijkstra(s);
      if (dist[t] == inf) return -inf;
      update(s, t, f, res);
    }
    return res;
  }
};

auto solve() {
  const int n = in, K = in;
  vector<i64> a(n);
  MinCostFlowDAG<int, i64> g(n);
  REP(i, n) {
    a[i] = in;
    int m = in;
    REP(j, m) {
      int b = in;
      --b;
      i64 cost = a[b] - a[i];
      if (cost < 0) {
        g.add_edge(b, i, 1, cost);
      }
    }
  }
  REP(i, n - 1) {
    g.add_edge(i, i + 1, K, 0);
  }
  return -g.solve(0, n - 1, K);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto ans = solve();
  print(ans);
}
