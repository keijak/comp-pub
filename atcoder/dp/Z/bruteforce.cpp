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

#define chmin(x, y) x = min(x, y)

const i64 INF = 1e8;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  i64 C;
  cin >> n >> C;
  vector<i64> H(n);
  REP(i, n) cin >> H[i];

  vector<i64> dp(n);

  auto jumpto = [&](int i) -> int {
    i64 x = H[i];
    i64 mincost = INF;
    int mj = n - 1;
    for (int j = i + 1; j < n; ++j) {
      i64 cost = H[j] * H[j] - 2 * H[j] * x + dp[j];
      if (mincost > cost) {
        mincost = cost;
        mj = j;
      }
    }
    return mj;
  };

  dp[n - 1] = 0;

  for (int i = n - 2; i >= 0; --i) {
    int j = jumpto(i);
    i64 hd = H[j] - H[i];
    dp[i] = hd * hd + C + dp[j];
  }
  cout << dp[0] << endl;
}
