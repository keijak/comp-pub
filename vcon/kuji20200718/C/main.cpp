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
  int H, W, K;
  cin >> H >> W >> K;
  vector<string> grid(H);
  REP(i, H) { cin >> grid[i]; }
  i64 ans = 0;
  for (int s = 0; s < (1 << (H + W)); ++s) {
    vector<string> g = grid;
    REP(r, H) {
      if (~s & (1 << r)) continue;
      REP(c, W) { g[r][c] = 'x'; }
    }
    REP(c, W) {
      if (~s & (1 << (c + H))) continue;
      REP(r, H) { g[r][c] = 'x'; }
    }
    int cnt = 0;
    REP(r, H) REP(c, W) {
      if (g[r][c] == '#') ++cnt;
    }
    if (cnt == K) ++ans;
  }
  cout << ans << endl;
}