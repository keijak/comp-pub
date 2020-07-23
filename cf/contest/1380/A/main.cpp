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

void solve() {
  int n;
  cin >> n;
  vector<int> p(n);
  for (auto& x : p) cin >> x;
  vector<int> rm(n + 1);
  rm[n] = n + 1;
  for (int i = n - 1; i >= 0; --i) {
    rm[i] = min(rm[i + 1], p[i]);
  }
  int m = p[0];
  int mi = 0;
  for (int i = 1; i < n; ++i) {
    if (p[i] > m && p[i] > rm[i + 1]) {
      cout << "YES\n";
      int k;
      for (k = i; k < n; ++k) {
        if (p[k] == rm[i + 1]) {
          break;
        }
      }
      cout << (mi + 1) << " " << (i + 1) << " " << (k + 1) << "\n";
      return;
    }
    if (m > p[i]) {
      m = p[i];
      mi = i;
    }
  }
  cout << "NO\n";
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
}
