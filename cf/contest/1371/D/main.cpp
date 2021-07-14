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

void solve() {
  int n, k;
  cin >> n >> k;
  vector<string> grid(n);
  REP(i, n) { grid[i] = string(n, '1'); }
  i64 ones = n * n;
  int r = 0, c = 0;
  int cstart = 0;
  for (; ones > k;) {
    grid[r][c] = '0';
    ones--;
    r++;
    c++;
    if (c == n) c = 0;
    if (r == n) {
      cstart++;
      c = cstart;
      r = 0;
    }
  }
  vector<i64> rs(n, 0), cs(n, 0);
  i64 rmax = 0, rmin = n, cmax = 0, cmin = n;
  REP(i, n) {
    REP(j, n) {
      int v = (grid[i][j] == '1') ? 1 : 0;
      rs[i] += v;
      cs[j] += v;
    }
  }
  REP(i, n) {
    rmax = max(rmax, rs[i]);
    rmin = min(rmin, rs[i]);
    cmax = max(cmax, cs[i]);
    cmin = min(cmin, cs[i]);
  }
  i64 fval = (rmax - rmin) * (rmax - rmin) + (cmax - cmin) * (cmax - cmin);
  cout << fval << '\n';
  REP(i, n) { cout << grid[i] << '\n'; }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    solve();
  }
}
