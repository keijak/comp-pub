#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define debug(...)
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  vector<vector<int>> adj(N);
  REP(i, M) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    adj[a].push_back(b);
    adj[b].push_back(a);
  }
  vector<bool> visited[2] = {vector<bool>(N), vector<bool>(N)};
  deque<pair<int, int>> q;
  visited[0][0] = true;
  q.emplace_back(0, 0);
  while (q.size()) {
    int d = q.front().first;
    int j = q.front().second;
    q.pop_front();
    for (auto x : adj[j]) {
      if (visited[(d + 1) % 2][x]) continue;
      visited[(d + 1) % 2][x] = true;
      q.emplace_back(d + 1, x);
    }
  }
  i64 ecnt = 0, ocnt = 0;
  REP(i, N) {
    if (visited[1][i]) {
      ocnt++;
    } else if (visited[0][i]) {
      ecnt++;
    }
  }

  i64 ans = 0;
  REP(i, N) {
    if (visited[0][i]) ecnt--;
    if (visited[1][i]) ocnt--;
    if (visited[0][i]) {
      ans += ocnt;
      for (auto j : adj[i]) {
        if (j > i && visited[1][j]) ans--;
      }
    } else if (visited[1][i]) {
      ans += ecnt;
      for (auto j : adj[i]) {
        if (j > i && visited[0][j]) ans--;
      }
    }
  }

  cout << ans << endl;
}
