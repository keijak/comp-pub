#include <vector>
#include <limits>
#include <cstdio>
using namespace std;
typedef long long i64;

const i64 BIG = numeric_limits<i64>::max();

struct Edge {
  i64 cost;
  int to;
  Edge(i64 c,int t): cost(c),to(t){}
};

// Finds the shortest path from the start node and detects negative cycle.
// Returns the distance from the start node to each node.
// INF indicates unreachable. -INF indicates having a negative cycle in a
// path from the start node.
i64 bellman_ford(const std::vector<std::vector<Edge> > &adj, const int start,
                 const int goal) {
  const int n = int(adj.size());
  std::vector<i64> mincost(n, BIG);  // distance from the start node.
  mincost[start] = 0;
  for (int k = 0; k < n - 1; ++k) {
    for (int i = 0; i < n; ++i) {
      const i64 di = mincost[i];
      if (di == BIG) continue;  // We haven't reached i yet.
      for (int j = 0; j < (int)adj[i].size(); ++j) {
        const Edge& e = adj[i][j];
        if (mincost[e.to] > di + e.cost) {
          mincost[e.to] = di + e.cost;
        }
      }
    }
  }

  // Negative cycle detection.
  // If there's no negative cycle, at least one node gets the shortest
  // distance determined in each iteration. If we have gone through N-1
  // iterations and still have an update, there must be a negative cycle.
  for (int k = 0; k < n + 5; ++k) {
    for (int i = 0; i < n; ++i) {
      const i64 di = mincost[i];
      if (di == BIG) continue;
      for (int j = 0; j < (int)adj[i].size(); ++j) {
        const Edge& e = adj[i][j];
        if (mincost[e.to] > di + e.cost) {
          return -1;
        }
      }
    }
  }
  if (mincost[goal] == BIG) return -2;
  return mincost[goal];
}

int main() {
  int n, ml, md;
  scanf("%d%d%d", &n, &ml, &md);

  vector<vector<Edge> > g(n);
  for (int i = 0; i < ml; ++i) {
    int a, b, d;
    scanf("%d%d%d", &a, &b, &d);
    --a, --b;
    g[a].push_back(Edge(d, b));
  }
  for (int i = 0; i < md; ++i) {
    int a, b, d;
    scanf("%d%d%d", &a, &b, &d);
    --a, --b;
    g[b].push_back(Edge(-d, a));
  }
  i64 ans = bellman_ford(g, 0, n - 1);
  printf("%lld\n", ans);
}
