#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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
  int n;
  cin >> n;
  V<set<int>> rows(n), cols(n);
  V<V<int>> a(n, V<int>(n));
  for (int k = 1; k <= n; ++k) {
    int r = k - 1, c = k - 1;
    while (a[r][c] == 0 && !rows[r].count(k) && !cols[c].count(k)) {
      a[r][c] = k;
      rows[r].insert(k);
      cols[c].insert(k);
      c = (c + 1) % n;
      r = (r + 2) % n;
    }
  }
  REP(i, n) {
    REP(j, n) { cout << a[i][j] << (j == n - 1 ? '\n' : ' '); }
  }
}
