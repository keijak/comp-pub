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

#define chmax(x, y) x = max(x, y)

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<tuple<int, int, int, i64>> blocks(N);
  REP(i, N) {
    int w, s, v;
    cin >> w >> s >> v;
    blocks[i] = {s + w, s, w, v};
  }
  sort(blocks.rbegin(), blocks.rend());

  const int SMAX = 1e4 + 5;
  vector<i64> dp(SMAX, -1);
  REP(i, N) {
    auto [_, s, w, v] = blocks[i];
    for (int s2 = w; s2 < SMAX; ++s2) {
      if (dp[s2] >= 0) {
        chmax(dp[min(s2 - w, s)], dp[s2] + v);
      }
    }
    chmax(dp[s], v);
  }
  i64 ans = 0;
  REP(s, SMAX) { chmax(ans, dp[s]); }
  cout << ans << endl;
}
