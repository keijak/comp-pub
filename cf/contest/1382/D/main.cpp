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
  int n;
  cin >> n;
  vector<int> p(2 * n);
  REP(i, 2 * n) cin >> p[i];
  vector<int> w;
  for (int i = 0; i < 2 * n;) {
    int clen = 1;
    for (int j = i + 1; j < 2 * n && p[j] < p[i]; ++j) {
      ++clen;
    }
    if (clen > n) return false;
    w.push_back(clen);
    i += clen;
  }
  //   REP(i, w.size()) { cerr << w[i] << " "; }
  //   cerr << endl;
  // knapsack til n
  vector<bool> dp(n + 1);
  dp[w[0]] = true;
  for (int i = 1; i < int(w.size()); ++i) {
    vector<bool> tmp = dp;
    int x = w[i];
    for (int j = n; j >= 0; --j) {
      if (j + x <= n && tmp[j]) {
        dp[j + x] = true;
      }
    }
  }
  return dp[n];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    cout << (solve() ? "YES\n" : "NO\n");
  }
}
