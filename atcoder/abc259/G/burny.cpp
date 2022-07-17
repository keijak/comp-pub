// #define NDEBUG
#include <bits/stdc++.h>
#include <atcoder/maxflow>

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

class UnionFindTree {
  struct base_node {
    int parent;
    int rank;
    int size;
  };

  vector<base_node> node;

 public:
  UnionFindTree(int n) {
    node.resize(n);
    for (int i = 0; i < n; i++) {
      node[i].parent = i;
      node[i].rank = 0;
      node[i].size = 1;
    }
  }

  int find(int x) {  // return root node of x
    if (node[x].parent == x)
      return x;
    else {
      return node[x].parent = find(node[x].parent);
    }
  }

  bool same(int x, int y) { return find(x) == find(y); }

  int size(int at) { return node[find(at)].size; }

  void unite(int x, int y) {
    x = find(node[x].parent);
    y = find(node[y].parent);

    if (x == y) return;

    if (node[x].rank < node[y].rank) {
      node[x].parent = y;
      node[y].size += node[x].size;
    } else if (node[x].rank > node[y].rank) {
      node[y].parent = x;
      node[x].size += node[y].size;
    } else {
      node[x].rank++;
      unite(x, y);
    }
  }
};

namespace max_flow {
using Integer = long long;
constexpr Integer INF = 1LL << 58;

typedef struct {
  int to;
  Integer cap;
  int rev;
  bool is_rev;
} edge;

// node v : distance from s => level[v]
void bfs(vector<vector<edge>> &G, vector<int> &level, int s) {
  fill(level.begin(), level.end(), -1);
  queue<int> q;
  q.push(s);
  level[s] = 0;
  while (!q.empty()) {
    int e = q.front();
    q.pop();
    for (int i = 0; i < (int) G[e].size(); i++) {
      if (G[e][i].cap > 0 && level[G[e][i].to] < 0) {
        level[G[e][i].to] = level[e] + 1;
        q.push(G[e][i].to);
      }
    }
  }
}

Integer dfs(vector<vector<edge>> &G, vector<int> &level, vector<bool> &used,
            vector<int> &iter, int s, Integer f, int t) {
  if (s == t)
    return f;
  else {
    // iter[e] : done searching from v[0] to v[ iter[e]-1 ]
    for (int &i = iter[s]; i < (int) G[s].size(); i++) {
      // distance from s to v[e][i].to must be longer than dist from s to v
      if (G[s][i].cap > 0 && level[s] < level[G[s][i].to]) {
        Integer d =
            dfs(G, level, used, iter, G[s][i].to, min(f, G[s][i].cap), t);
        if (d > 0) {
          G[s][i].cap -= d;
          G[G[s][i].to][G[s][i].rev].cap += d;
          return d;
        }
      }
    }
    return 0;
  }
}

Integer dinic_maxflow(vector<vector<edge>> &G, int s, int t) {
  Integer flow = 0;
  while (true) {
    vector<int> level(G.size(), -1);
    vector<int> iter(G.size(), 0);
    vector<bool> used(G.size(), false);
    bfs(G, level, s);
    if (level[t] < 0) return flow;  // unable to achieve to t
    while (true) {
      Integer f = dfs(G, level, used, iter, s, INF, t);
      if (f == 0)
        break;
      else
        flow += f;
    }
  }
}

void add_edge(vector<vector<edge>> &G, int from, int to, Integer cap) {
  G[from].push_back((edge) {to, cap, (int) G[to].size(), false});
  G[to].push_back((edge) {from, 0, (int) G[from].size() - 1, true});
}

}  // namespace max_flow

// to burn or to bury problem solver
namespace burny {

struct Variable {
  size_t id;
  int x = 1;

  Variable operator~() {
    Variable res;
    res.id = id;
    res.x = 1 - x;
    return res;
  }

  bool operator<(const Variable &v) const {
    return id == v.id ? x < v.x : id < v.id;
  }
};

enum class State {
  INFEASIBLE,
  NOT_SOLVED,
  OPTIMAL,
};

ostream &operator<<(ostream &os, const State &state) {
  if (state == State::INFEASIBLE) return os << "INFEASIBLE";
  if (state == State::NOT_SOLVED) return os << "NOT_SOLVED";
  if (state == State::OPTIMAL) return os << "OPTIMAL";
  return os;
}

// minimize a + sum_i c_{i1} * x_i + c_{i0} * (1-x_i) + sum_{i,j} c_{i1,j0} x_i
// * (1-x_j) x_i = {0, 1}
template<class Integer>
class Model {
  vector<Variable> variables;
  Integer cost_d0;

  using table_2 = array<Integer, 2>;
  map<size_t, table_2> cost_d1;

  using table_4 = array<Integer, 4>;
  map<pair<size_t, size_t>, table_4> cost_d2;

 public:
  State state = State::NOT_SOLVED;
  Integer answer;
  vector<int> assignments;

  Variable add_variable() {
    size_t id = variables.size();
    Variable v;
    v.id = id;
    v.x = 1;
    variables.emplace_back(v);
    return variables.back();
  }

  void add_constraint(Variable x, Integer cost) { cost_d1[x.id][x.x] += cost; }

  void add_constraint(Variable x, Variable y, Integer cost) {
    if (y < x) swap(x, y);
    assert(x.id != y.id);

    int t = (x.x << 1) | (y.x << 0);
    cost_d2[{x.id, y.id}][t] += cost;
  }

  pair<Integer, State> solve() {
    assert(state == State::NOT_SOLVED);
    cost_d0 = 0;

    int n = variables.size();
    UnionFindTree uft(n * 2);
    for (auto &c: cost_d2) {
      size_t i, j;
      tie(i, j) = c.first;
      const auto &t = c.second;

      Integer s = -t[0b00] + t[0b01] + t[0b10] - t[0b11];

      if (s < 0) {  // must flip
        uft.unite(i, j + n);
        uft.unite(i + n, j);
      } else if (s > 0) {
        uft.unite(i, j);
        uft.unite(i + n, j + n);
      } else {
        // s = 0, any flip
      }
    }

    // find feasible flip
    vector<int> flip(n, -1);
    {
      for (int i = 0; i < n; i++) {
        if (uft.same(i, i + n)) {
          state = State::INFEASIBLE;
          return {0, State::INFEASIBLE};
        }
      }

      map<int, vector<int>> s;
      for (int i = 0; i < n + n; i++) s[uft.find(i)].push_back(i);

      for (auto &w: s) {
        int r = w.first;
        if (flip[r % n] != -1) continue;
        for (auto v: w.second) {
          flip[v % n] = v >= n;
        }
      }
    }

    for (auto &c: cost_d2) {
      size_t i, j;
      tie(i, j) = c.first;
      auto &t = c.second;
      if (flip[i]) {
        swap(t[0b00], t[0b10]);
        swap(t[0b01], t[0b11]);
      }
      if (flip[j]) {
        swap(t[0b00], t[0b01]);
        swap(t[0b10], t[0b11]);
      }
      //     j=0 j=1
      // i=0 | a b = a + 0     0     + 0 (d-c) + 0 (c+b-a-d)
      // i=1 | c d       (c-a) (c-a)   0 (d-c)   0 0
      Integer s = -t[0b00] + t[0b01] + t[0b10] - t[0b11];
      cost_d0 += t[0b00];
      cost_d1[i][flip[i] == 0 ? 1 : 0] += t[0b10] - t[0b00];
      cost_d1[j][flip[j] == 0 ? 1 : 0] += t[0b11] - t[0b10];
      t[0b01] = s;
      t[0b00] = t[0b10] = t[0b11] = 0;
      assert(s >= 0);
    }

    for (auto &c: cost_d1) {
      size_t id = c.first;
      auto &t = c.second;
      if (flip[id]) {
        swap(t[0], t[1]);
      }
      if (t[1] >= t[0]) {
        cost_d0 += t[0];
        t[1] -= t[0];
        t[0] = 0;
      } else {
        cost_d0 += t[1];
        t[0] -= t[1];
        t[1] = 0;
      }
      assert(t[0] >= 0);
      assert(t[1] >= 0);
    }

    // make graph
    vector<vector<max_flow::edge>> g(n + 2);
    int source = n + 0;
    int sink = source + 1;

    for (auto &c: cost_d1) {
      size_t id = c.first;
      auto &t = c.second;
      if (t[1] > 0) {
        max_flow::add_edge(g, id, sink, t[1]);
      }
      if (t[0] > 0) {
        max_flow::add_edge(g, source, id, t[0]);
      }
    }

    for (auto &c: cost_d2) {
      size_t i, j;
      tie(i, j) = c.first;
      const auto &t = c.second;

      if (t[0b10] > 0) {
        max_flow::add_edge(g, i, j, t[0b10]);
      }
      if (t[0b01] > 0) {
        max_flow::add_edge(g, j, i, t[0b01]);
      }
    }

    // solve max flow (min cut)
    Integer cut = max_flow::dinic_maxflow(g, source, sink);
    answer = cut + cost_d0;
    state = State::OPTIMAL;

    // assignment
    assignments = vector<int>(n, 0);
    {
      // find S set
      set<int> visit;
      queue<int> q;
      visit.insert(source);
      q.push(source);
      while (q.size()) {
        int v = q.front();
        q.pop();
        for (auto &e: g[v]) {
          if (e.is_rev) continue;
          if (e.cap == 0) continue;
          if (visit.count(e.to)) continue;
          visit.insert(e.to);
          q.push(e.to);

          assignments[e.to] = 1;
        }
      }
    }
    for (int i = 0; i < n; i++) {
      if (flip[i]) {
        assignments[i] = 1 - assignments[i];
      }
    }

    return {answer, state};
  }
};

}  // namespace burny

auto solve() {
  int H = in, W = in;
  auto A = vector(H, vector(W, 0LL));
  REP(i, H) {
    REP(j, W) {
      cin >> A[i][j];
    }
  }
  const Int bigcost = 1e14;
  burny::Model<Int> model;
  vector<burny::Variable> row, col;
  REP(i, H) row.push_back(model.add_variable());
  REP(j, W) col.push_back(model.add_variable());

  REP(i, H) {
    REP(j, W) {
      if (A[i][j] < 0) {
        model.add_constraint(row[i], col[j], bigcost);
      } else if (A[i][j] > 0) {
//        model.add_constraint(row[i], -A[i][j]);
//        model.add_constraint(col[j], -A[i][j]);
        model.add_constraint(row[i], col[j], A[i][j]);
        model.add_constraint(row[i], col[j], A[i][j]);
        model.add_constraint(row[i], col[j], A[i][j]);
      }
    }
  }
  auto ans = model.solve();
  DUMP(ans.second);
  out(ans.first);
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
