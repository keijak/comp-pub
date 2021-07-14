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

vector<pair<i64, int>> factorize(i64 n) {
  vector<pair<i64, int>> res;
  for (i64 k = 2; k * k <= n; ++k) {
    int cnt = 0;
    while (n % k == 0) {
      n /= k;
      ++cnt;
    }
    if (cnt > 0) {
      res.emplace_back(k, cnt);
    }
  }
  if (n > 1) {
    res.emplace_back(n, 1);
  }
  return res;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<i64> t(n);
  REP(i, n) cin >> t[i];
  map<i64, int> mp;
  REP(i, n) {
    for (auto [p, k] : factorize(t[i])) {
      mp[p] = max(mp[p], k);
    }
  }
  i64 ans = 1;
  for (auto [p, k] : mp) {
    REP(j, k) { ans *= p; }
  }
  cout << ans << endl;
}
