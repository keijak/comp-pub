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

const int INF_DIST = 10000000;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  // Input
  int N, M;
  cin >> N >> M;
  vector<vector<int>> to(N);
  REP(i, M) {
    int u, v;
    cin >> u >> v;
    u--;
    v--;
    to[u].push_back(v);
    to[v].push_back(u);
  }
  int start;
  cin >> start;
  start--;
  int K;
  cin >> K;
  vector<int> targets(K);
  for (auto& x : targets) {
    cin >> x;
    x--;
  }

  // BFS
  vector<vector<i64>> dist(K + 1, vector<i64>(K, INF_DIST));
  REP(i, K + 1) {
    deque<tuple<int, int>> q;
    set<int> visited;
    int st = (i == K) ? start : targets[i];
    visited.insert(st);
    q.emplace_back(0, st);
    int target_counter = 0;
    while (q.size()) {
      auto [dcur, cur] = q.front();
      q.pop_front();
      auto it = find(targets.begin(), targets.end(), cur);
      if (it != targets.end()) {
        int j = distance(targets.begin(), it);
        DEBUG(st, i, cur, j, dcur);
        dist[i][j] = dcur;
        if (++target_counter == K) {
          break;
        }
      }
      for (auto x : to[cur]) {
        if (visited.count(x)) continue;
        visited.insert(x);
        q.emplace_back(dcur + 1, x);
      }
    }
  }

  // Bit DP
  // dp[x][y] = minimum cost to finish travel.
  // x: the set of unvisited nodes
  // y: current node
  const i64 L = 1LL << K;
  vector<vector<int>> dp(L, vector<int>(K, INF_DIST));
  REP(i, K) dp[0][i] = 0;
  REP(unvisited, L) {
    REP(next_visit, K) {
      if (!(unvisited & (1 << next_visit))) continue;
      int next_state = unvisited & ~(1 << next_visit);
      REP(cur, K) {
        i64 new_cost = dist[cur][next_visit] + dp[next_state][next_visit];
        if (dp[unvisited][cur] > new_cost) {
          dp[unvisited][cur] = new_cost;
        }
      }
    }
  }

  // Output
  int ans = INF_DIST;
  REP(i, K) {
    int d = dist[K][i] + dp[L - 1][i];
    if (ans > d) ans = d;
  }
  cout << ans << endl;
}
