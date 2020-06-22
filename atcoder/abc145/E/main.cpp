#define ENABLE_DEBUG 1
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if ENABLE_DEBUG
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
#else
#define debug(...)
#define DEBUG(...)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, T;
  cin >> N >> T;
  vector<pair<int, int>> AB(N);
  REP(i, N) { cin >> AB[i].first >> AB[i].second; }
  sort(AB.begin(), AB.end());
  i64 bmax = 0;
  vector<i64> dp(3001, -1);
  REP(i, N) {
    dp[0] = 0;
    int a = AB[i].first;
    int b = AB[i].second;
    for (int t = T - 1; t >= 0; --t) {
      if (dp[t] >= 0) {
        bmax = max(bmax, dp[t] + b);
        if (t + a < T) {
          dp[t + a] = max(dp[t + a], dp[t] + b);
        }
      }
    }
  }
  cout << bmax << '\n';
}
