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
  int n;
  cin >> n;
  string s;
  cin >> s;
  vector<i64> lsums(n + 1);
  vector<i64> rsums(n + 1);
  REP(i, n) { lsums[i + 1] = lsums[i] + int(s[i] == 'W'); }
  REP(i, n) {
    int j = n - 1 - i;
    rsums[j] = rsums[j + 1] + int(s[j] == 'E');
  }
  const i64 INF = 1LL << 50;
  i64 mincost = INF;
  REP(i, n) {
    i64 cost = lsums[i] + rsums[i + 1];
    mincost = min(mincost, cost);
  }
  cout << mincost << endl;
}