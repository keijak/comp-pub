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
  int n, m;
  cin >> n >> m;
  set<int> a, b;
  REP(i, n) {
    int x;
    cin >> x;
    a.insert(x);
  }
  REP(i, m) {
    int x;
    cin >> x;
    b.insert(x);
  }
  set<int> res;
  set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                   inserter(res, res.begin()));
  if (res.empty()) {
    cout << "NO\n";
    return;
  }
  cout << "YES\n";
  cout << 1 << " " << (*res.begin()) << "\n";
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
