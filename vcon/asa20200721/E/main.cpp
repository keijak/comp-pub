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
  int n, k;
  cin >> n >> k;
  vector<i64> v(n);
  for (auto& x : v) cin >> x;
  multiset<i64> lb;
  i64 lbsum = 0;
  i64 ans = 0;
  for (int i = 0; i <= min(k, n); ++i) {
    if (i > 0) {
      lb.insert(v[i - 1]);
      lbsum += v[i - 1];
    }
    multiset<i64> rb(lb.begin(), lb.end());
    i64 rbsum = lbsum;
    for (int j = 0; j <= min(k, n) - i; ++j) {
      if (j > 0) {
        rb.insert(v[n - j]);
        rbsum += v[n - j];
      }
      int p = k - i - j;
      auto it = rb.begin();
      i64 value = rbsum;
      for (int r = 0; r < p && it != rb.end(); ++r, ++it) {
        if (*it >= 0) break;
        value -= *it;
      }
      ans = max(ans, value);
    }
  }
  cout << ans << endl;
}
