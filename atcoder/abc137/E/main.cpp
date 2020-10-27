//#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#endif

const i64 INF = 1LL << 40;

// Find shortest path from a start node.
// Allows edges with negative cost.
pair<vector<i64>, bool> bellman_ford(const int start,
                                     const vector<vector<pair<int, i64>>>& adj,
                                     const vector<bool>& reach_s,
                                     const vector<bool>& reach_e) {
  const int N = static_cast<int>(adj.size());
  vector<i64> dist(N, INF);  // distance from the start node.
  dist[start] = 0;

  for (int k = 0;; ++k) {
    bool update = false;
    REP(i, N) {
      if (dist[i] == INF) {
        continue;  // We haven't reached i yet.
      }
      if (!reach_s[i] || !reach_e[i]) continue;
      for (const pair<int, i64>& edge : adj[i]) {
        int j = edge.first;  // from i to j
        if (!reach_s[j] || !reach_e[j]) continue;
        i64 cost = edge.second;
        if (dist[j] > dist[i] + cost) {
          dist[j] = dist[i] + cost;
          update = true;
        }
      }
    }
    if (!update) {
      return {std::move(dist), false};
    }
    // Negative cycle detection.
    // When there's no negative cycle, at least one node gets the shortest
    // distance determined for each iteration. If we have gone through N+1
    // iterations and still have an update, there must be a negative cycle.
    if (k == N) {
      return {std::move(dist), true};
    }
  }
  // Never reach here.
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M, P;
  cin >> N >> M >> P;
  vector<vector<pair<int, i64>>> cost(N);   // negative.
  vector<vector<pair<int, i64>>> rcost(N);  // negative.
  REP(i, M) {
    int a, b, c;
    cin >> a >> b >> c;
    a--;
    b--;
    cost[a].emplace_back(b, -(c - P));
    rcost[b].emplace_back(a, -(c - P));
  }
  auto reach_s = vector<bool>(N);
  auto dfs1 = [&](auto rec, int cur) -> void {
    reach_s[cur] = true;
    for (auto& edge : cost[cur]) {
      int i = edge.first;
      if (!reach_s[i]) {
        rec(rec, i);
      }
    }
  };
  dfs1(dfs1, 0);

  auto reach_e = vector<bool>(N);
  auto dfs2 = [&](auto rec, int cur) -> void {
    reach_e[cur] = true;
    for (auto& edge : rcost[cur]) {
      int i = edge.first;
      if (!reach_e[i]) {
        rec(rec, i);
      }
    }
  };
  dfs2(dfs2, N - 1);

  auto result = bellman_ford(0, cost, reach_s, reach_e);
  const vector<i64>& dist = result.first;
  const bool has_cycle = result.second;
  if (has_cycle) {
    cout << -1 << '\n';
  } else {
    assert(dist[N - 1] != INF);
    cout << max(-dist[N - 1], 0LL) << '\n';
  }
}
