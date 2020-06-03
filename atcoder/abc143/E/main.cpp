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

const int INF = 500;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  i64 L;
  cin >> N >> M >> L;
  vector<vector<i64>> mincost(N, vector<i64>(N, 1LL << 50));
  REP(i, M) {
    int a, b, c;
    cin >> a >> b >> c;
    a--;
    b--;
    mincost[a][b] = mincost[b][a] = c;
  }
  REP(k, N) REP(i, N) REP(j, N) {
    i64 c = mincost[i][k] + mincost[k][j];
    if (mincost[i][j] > c) mincost[i][j] = c;
  }

  vector<vector<i64>> mincost2(N, vector<i64>(N, INF));
  REP(i, N) REP(j, N) {
    if (mincost[i][j] <= L) {
      mincost2[i][j] = 1;
    }
  }
  REP(k, N) REP(i, N) REP(j, N) {
    i64 c = mincost2[i][k] + mincost2[k][j];
    if (mincost2[i][j] > c) mincost2[i][j] = c;
  }

  int Q;
  cin >> Q;
  REP(i, Q) {
    int s, t;
    cin >> s >> t;
    s--;
    t--;
    i64 refill = mincost2[s][t];
    cout << (refill == INF ? -1 : refill - 1) << '\n';
  }
}
