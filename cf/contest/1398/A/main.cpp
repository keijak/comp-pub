#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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

void solve() {
  int n;
  cin >> n;
  V<i64> a(n);
  REP(i, n) cin >> a[i];
  if (a[0] + a[1] > a[n - 1]) {
    cout << -1 << '\n';
    return;
  } else {
    cout << 1 << ' ' << 2 << ' ' << n << '\n';
  }
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) solve();
}
