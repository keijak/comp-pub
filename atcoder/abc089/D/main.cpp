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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int H, W, D;
  cin >> H >> W >> D;
  vector<vector<int>> A(H, vector<int>(W));
  vector<pair<int, int>> loc(H * W + 1);
  REP(i, H) REP(j, W) {
    cin >> A[i][j];
    loc[A[i][j]] = {i, j};
  }
  vector<i64> costs(H * W + 1, -1);
  for (int v = H * W; v > 0; --v) {
    if (costs[v] >= 0) continue;
    int p = v % D;
    i64 cost = 0;
    costs[p] = cost;
    pair<int, int> ploc = loc[p];
    while (p != v) {
      int x = p + D;
      pair<int, int> xloc = loc[x];
      cost += abs(xloc.first - ploc.first) + abs(xloc.second - ploc.second);
      costs[x] = cost;
      p = x;
      ploc = xloc;
    }
  }
  int Q;
  cin >> Q;
  while (Q--) {
    int l, r;
    cin >> l >> r;
    cout << (costs[r] - costs[l]) << '\n';
  }
}