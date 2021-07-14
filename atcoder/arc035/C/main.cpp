#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void _debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void _debug(T value, Ts... args) {
  std::cerr << value << ", ";
  _debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    _debug(__VA_ARGS__);               \
    cerr << endl;                      \
  } while (0)
#endif

const int INF = 1'000'000;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  vector<vector<int>> dist(N, vector<int>(N, INF));
  REP(i, N) dist[i][i] = 0;
  REP(i, M) {
    int a, b, c;
    cin >> a >> b >> c;
    a--;
    b--;
    dist[a][b] = dist[b][a] = c;
  }
  REP(k, N) REP(i, N) REP(j, N) {
    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
  }
  int K;
  cin >> K;
  REP(k, K) {
    int x, y, z;
    cin >> x >> y >> z;
    x--;
    y--;
    if (z < dist[x][y]) {
      // Update.
      dist[x][y] = z;
      REP(i, N) REP(j, N) {
        int d = min(dist[i][y] + z + dist[x][j], dist[i][x] + z + dist[y][j]);
        dist[i][j] = min(dist[i][j], d);
      }
    }
    i64 dsum = 0;
    REP(i, N) REP(j, i) { dsum += dist[i][j]; }
    cout << dsum << '\n';
  }
}
