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

i64 dsum(i64 x) {
  i64 res = 0;
  while (x) {
    res += x % 10;
    x /= 10;
  }
  return res;
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N;
  cin >> N;

  i64 tmp = N;
  i64 dsum_max = 0;
  while (tmp) {
    dsum_max += 9;
    tmp /= 10;
  }

  vector<i64> ans;
  for (int d = 0; d <= dsum_max; ++d) {
    i64 x = N - d;
    if (x <= 0) continue;
    if (dsum(x) == d) {
      ans.push_back(x);
    }
  }
  sort(ans.begin(), ans.end());
  cout << ans.size() << '\n';
  for (auto x : ans) cout << x << '\n';
}
