#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void _debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void _debug(T value, Ts... args) {
  std::cerr << value << ", ";
  _debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    _debug(__VA_ARGS__);               \
    cerr << endl;                      \
  } while (0)
#endif

i64 solve(i64 n, i64 x, int m, vector<pair<i64, i64>>& lr) {
  i64 al = x, ar = x;
  for (auto [l, r] : lr) {
    if (r < al || l > ar) continue;
    al = min(al, l);
    ar = max(ar, r);
  }
  return ar - al + 1;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    i64 n, x;
    int m;
    cin >> n >> x >> m;
    x--;
    vector<pair<i64, i64>> lr(m);
    REP(i, m) {
      i64 l, r;
      cin >> l >> r;
      l--;
      r--;
      lr[i] = make_pair(l, r);
    }
    cout << solve(n, x, m, lr) << '\n';
  }
}