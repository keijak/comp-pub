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

bool solve() {
  i64 n, k, l;
  cin >> n >> k >> l;
  vector<i64> d(n);
  REP(i, n) cin >> d[i];

  i64 m = 2 * k;
  vector<i64> p(m);
  REP(i, k) { p[i] = i; }
  REP(i, k) { p[m - 1 - i] = 1 + i; }
  auto depth = [&](int i, i64 t) -> i64 { return d[i] + p[t % m]; };
  vector<vector<bool>> dp(n + 1, vector<bool>(m));
  REP(i, 2 * k) { dp[0][i] = true; }
  for (int i = 1; i <= n; ++i) {
    for (int t = 0; t < m; ++t) {
      dp[i][t] =
          dp[i][t] || (dp[i - 1][(t - 1 + m) % m] && depth(i - 1, t) <= l);
    }
    for (int t = 0; t < m; ++t) {
      dp[i][t] = dp[i][t] || (dp[i][(t - 1 + m) % m] && depth(i - 1, t) <= l);
    }
  }
  REP(t, m) {
    if (dp[n][t]) return true;
  }
  return false;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    cout << (solve() ? "Yes\n" : "No\n");
  }
}