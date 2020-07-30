#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

const i64 INF = 1LL << 50;

template <class Monoid>
struct SegTree {
  using Func = function<Monoid(Monoid, Monoid)>;
  const Func F;
  const Monoid UNITY;
  int size;
  vector<Monoid> dat;

  SegTree(int n, const Func f, const Monoid &unity)
      : F(f), UNITY(unity), size(n), dat(2 * n, unity) {}

  // Sets i-th value (0-indexed) to x for initial setup.
  // build() must be called after set() calls.
  void set(int i, const Monoid &x) { dat[size + i] = x; }
  void build() {
    for (int k = size - 1; k > 0; --k) {
      dat[k] = F(dat[k * 2], dat[k * 2 + 1]);
    }
  }

  // Sets i-th value (0-indexed) to x.
  void update(int i, const Monoid &x) {
    int k = size + i;
    dat[k] = x;
    while (k > 1) {
      k >>= 1;
      dat[k] = F(dat[k * 2], dat[k * 2 + 1]);
    }
  }

  // Queries by [l,r) range (0-indexed, open interval).
  Monoid fold(int l, int r) {
    l += size;
    r += size;
    Monoid vleft = UNITY, vright = UNITY;
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vleft = F(vleft, dat[l++]);
      if (r & 1) vright = F(dat[--r], vright);
    }
    return F(vleft, vright);
  }

  // Queries by a single index (0-indexed).
  Monoid operator[](int i) { return dat[size + i]; }

  /* debug */
  void print() {
    for (int i = 0; i < size; ++i) {
      cout << (*this)[i];
      if (i != size - 1) cout << ",";
    }
    cout << endl;
  }
};

i64 solve() {
  i64 N, M, A, B;
  cin >> N >> M >> A >> B;
  if (A == B) return 0;
  DEBUG(N, M, A, B);
  A--;
  B--;
  vector<i64> C(N);
  vector<int> parent(N, -1);
  vector<int> depth(N, -1);
  vector<vector<int>> g(N);
  REP(i, N) {
    i64 p, c;
    cin >> p >> c;
    C[i] = (c == 0) ? INF : c;
    --p;
    parent[i] = p;
    if (p >= 0) {
      g[i].push_back(p);
      g[p].push_back(i);
    }
  }
  C[A] = 0;
  depth[0] = 0;
  auto calc_depth = [&](auto rec, int v) -> void {
    if (depth[v] != -1) return;
    int p = parent[v];
    assert(p >= 0);
    assert(p < N);
    if (depth[p] == -1) {
      rec(rec, p);
    }
    assert(depth[p] >= 0);
    depth[v] = depth[p] + 1;
  };
  REP(i, N) { calc_depth(calc_depth, i); }

  vector<bool> on_main_path(N);
  // lowest common ancestor.
  auto calc_lca = [&](int v, int u) -> int {
    on_main_path[v] = on_main_path[u] = true;
    while (depth[v] > depth[u]) {
      v = parent[v];
      on_main_path[v] = true;
    }
    while (depth[u] > depth[v]) {
      u = parent[u];
      on_main_path[u] = true;
    }
    while (v != u) {
      v = parent[v];
      u = parent[u];
      on_main_path[v] = on_main_path[u] = true;
    }
    return v;
  };
  calc_lca(A, B);

  vector<int> next_opt(N, -1);
  vector<int> goal_dist(N, N);  // distance from B.
  int max_dist = 0;

  auto bfs = [&]() {
    deque<tuple<int, int, int>> q;
    q.emplace_back(B, -1, 0);
    while (q.size()) {
      auto [v, p, d] = q.front();
      q.pop_front();
      goal_dist[v] = d;
      next_opt[v] = p;
      max_dist = max(max_dist, d);
      for (auto u : g[v]) {
        if (u != p) {
          q.emplace_back(u, v, d + 1);
        }
      }
    }
  };
  bfs();

  REP(i, N) {
    sort(g[i].begin(), g[i].end(),
         [&](int x, int y) { return goal_dist[x] > goal_dist[y]; });
  }

  vector<i64> reachcost(N, INF);  // node -> cost
  const i64 L = N + 100;
  SegTree<i64> fillcost(
      L, [](i64 x, i64 y) { return min(x, y); }, INF);

  auto dfs = [&](int v0, int p0, int root, int d0, map<int, int> &buf) -> void {
    int gdr = goal_dist[root];
    if (M < d0) return;
    stack<tuple<int, int, int>> s;
    s.emplace(v0, p0, d0);
    while (s.size()) {
      auto [v, p, d] = s.top();
      s.pop();
      i64 rc = fillcost.fold(gdr, min(gdr + M - d + 1, L));
      reachcost[v] = rc;
      DEBUG(v, p, rc, gdr, gdr + M - d);
      if (rc < INF && C[v] < INF) {
        int gdv = goal_dist[v];
        auto it = buf.find(gdv);
        if (it == buf.end() || it->second > rc + C[v]) {
          DEBUG(gdv, rc + C[v]);
          buf[gdv] = rc + C[v];
        }
      }
      for (auto u : g[v]) {
        if (u == p) continue;
        s.emplace(u, v, d + 1);
      }
    }
  };

  reachcost[A] = 0;
  fillcost.update(goal_dist[A], 0);
  // Walk along the main path.
  for (int v = next_opt[A];; v = next_opt[v]) {
    int gd = goal_dist[v];
    i64 rc = fillcost.fold(gd + 1, min(gd + 1 + M, L));
    reachcost[v] = rc;
    if (rc == INF) return -1;
    if (v == B) break;
    i64 curc = fillcost.fold(gd, gd + 1);
    if (rc < INF && C[v] < INF && curc > rc + C[v]) {
      fillcost.update(gd, rc + C[v]);
    }

    // Side walk.
    map<int, int> buf;
    for (int u : g[v]) {
      if (!on_main_path[u]) {
        dfs(u, v, v, 1, buf);
      }
    }
    for (auto [dd, fc] : buf) {
      i64 cc = fillcost.fold(dd, dd + 1);
      if (cc > fc) {
        fillcost.update(dd, fc);
      }
    }
  }

  i64 ans = reachcost[B];
  return (ans == INF) ? -1 : ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    cerr << "test " << (i + 1) << endl;
    DEBUG(i);
    cout << "Case #" << (i + 1) << ": " << solve() << "\n";
  }
}
