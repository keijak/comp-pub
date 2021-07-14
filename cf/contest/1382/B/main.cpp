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

bool solve() {
  i64 n;
  cin >> n;
  vector<i64> a(n);
  REP(i, n) cin >> a[i];
  bool losing = true;
  for (int i = n - 1; i >= 0; --i) {
    if (a[i] == 0) continue;
    if (losing) {
      losing = false;
    } else {
      if (a[i] == 1)
        losing = true;
      else
        losing = false;
    }
  }
  return !losing;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    cout << (solve() ? "First" : "Second") << "\n";
  }
}
