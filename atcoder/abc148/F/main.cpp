#define MY_DEBUG 0
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if MY_DEBUG
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                \
  do {                            \
    cerr << " \033[33m (L";       \
    cerr << __LINE__ << ") ";     \
    cerr << #__VA_ARGS__ << ": "; \
    cerr << "\033[0m";            \
    debug(__VA_ARGS__);           \
    cerr << endl;                 \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, escaper, chaser;
  cin >> N >> escaper >> chaser;
  escaper--;
  chaser--;
  vector<vector<int>> adj(N);
  REP(i, N - 1) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    adj[a].push_back(b);
    adj[b].push_back(a);
  }

  vector<int> depth(N, -1), parent(N, -1);
  depth[chaser] = 0;
  deque<tuple<int, int>> q;
  q.emplace_back(chaser, 0);
  while (q.size()) {
    auto [node, d] = q.front();
    q.pop_front();
    for (int x : adj[node]) {
      if (depth[x] != -1) continue;
      depth[x] = d + 1;
      parent[x] = node;
      q.emplace_back(x, d + 1);
    }
  }

  vector<int> maxdepth(N, -1);
  auto dfs = [&](auto rec, int i) -> int {
    if (maxdepth[i] != -1) return maxdepth[i];
    int md = depth[i];
    for (int x : adj[i]) {
      if (depth[x] > depth[i]) {
        int d = rec(rec, x);
        md = max(md, d);
      }
    }
    maxdepth[i] = md;
    return md;
  };
  const int height = dfs(dfs, chaser);

  vector<int> path;
  path.reserve(height);
  for (int i = escaper; i != chaser; i = parent[i]) {
    path.push_back(i);
  }
  path.push_back(chaser);

  int steps = 0;
  int e = escaper, c = chaser;
  REP(i, path.size()) {
    e = path[i], c = path[path.size() - 1 - i];
    if (depth[e] - 1 <= depth[c] + 1) break;
    steps++;
  }
  int dist = depth[e] - depth[c];
  assert(dist == 1 || dist == 2);
  steps += maxdepth[e] - depth[e];
  if (dist == 2) {
    steps++;
  }
  cout << steps << endl;
}
