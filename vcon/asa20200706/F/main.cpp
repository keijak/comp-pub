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
  int H, W, N;
  cin >> H >> W >> N;
  set<tuple<int, int>> black;
  REP(i, N) {
    int r, c;
    cin >> r >> c;
    black.insert({r, c});
  }
  map<tuple<int, int>, int> gsum;
  for (auto [r, c] : black) {
    for (int i = -2; i <= 0; ++i) {
      for (int j = -2; j <= 0; ++j) {
        int rx = r + i;
        int cx = c + j;
        if (rx <= 0 || rx > H - 2 || cx <= 0 || cx > W - 2) continue;
        gsum[{rx, cx}]++;
      }
    }
  }
  vector<i64> ans(10);
  u64 total = (u64)(H - 2) * (W - 2);
  for (auto [p, cnt] : gsum) {
    assert(cnt <= 9);
    ans[cnt]++;
    --total;
  }
  ans[0] = total;

  REP(i, 10) { cout << ans[i] << '\n'; }
}
