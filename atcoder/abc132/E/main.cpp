#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  vector<vector<int>> G(3 * N);
  REP(i, M) {
    int u, v;
    cin >> u >> v;
    u--;
    v--;
    G[3 * u].push_back(3 * v + 1);
    G[3 * u + 1].push_back(3 * v + 2);
    G[3 * u + 2].push_back(3 * v);
  }
  int S, T;
  cin >> S >> T;
  S--;
  T--;

  // BFS
  deque<tuple<int, int>> q;
  vector<bool> visited(3 * N);
  q.emplace_back(3 * S, 0);
  visited[S] = true;
  int ans = -1;
  while (q.size()) {
    auto [v, d] = q.front();
    q.pop_front();
    if (v == 3 * T) {
      assert(d % 3 == 0);
      ans = d / 3;
      break;
    }
    for (auto x : G[v]) {
      if (visited[x]) continue;
      visited[x] = true;
      q.emplace_back(x, d + 1);
    }
  }
  cout << ans << '\n';
}
