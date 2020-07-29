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

int leftmost_bit(unsigned int x) {
  int l = 31 - __builtin_clz(x);
  return 1 << l;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<vector<i64>> a(N, vector<i64>(N));
  REP(i, N) REP(j, N) { cin >> a[i][j]; }

  vector<i64> scores(1 << N);
  REP(s, 1 << N) {
    i64 score = 0;
    REP(i, N) {
      if (~s & 1 << i) continue;
      REP(j, i) {
        if (s & 1 << j) score += a[i][j];
      }
    }
    scores[s] = score;
  }

  vector<i64> dp(1 << N);
  vector<bool> done(1 << N);
  done[0] = true;
  auto maximize = [&](auto rec, int state) -> i64 {
    if (done[state]) return dp[state];
    int b = leftmost_bit(state);
    i64 res = 0;
    for (int s = b; s < (b << 1); ++s) {
      if ((s & state) == s) {
        i64 r = rec(rec, state & ~s) + scores[s];
        res = max(res, r);
      }
    }
    dp[state] = res;
    done[state] = true;
    return res;
  };
  i64 ans = maximize(maximize, (1 << N) - 1);
  cout << ans << endl;
}
