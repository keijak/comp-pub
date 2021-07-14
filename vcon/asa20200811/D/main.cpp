#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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
  int n;
  cin >> n;
  V<int> a(n);
  REP(i, n) cin >> a[i];
  V<i64> dp(n + 1);
  dp[1] = abs(a[0] - a[1]);
  for (int i = 2; i < n; ++i) {
    dp[i] = (min(dp[i - 1] + abs(a[i] - a[i - 1]),
                 dp[i - 2] + abs(a[i] - a[i - 2])));
  }
  cout << dp[n - 1] << endl;
}
