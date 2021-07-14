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
  int N;
  cin >> N;
  vector<vector<i64>> digits(10, vector<i64>(10));
  for (int i = 1; i <= N; ++i) {
    int d1 = i % 10;
    int d2 = d1;
    int x = i;
    while (x) {
      d2 = x % 10;
      x /= 10;
    }
    if (d1 != 0 && d2 != 0) {
      digits[d1][d2]++;
    }
  }
  i64 ans = 0;
  REP(i, 10) REP(j, 10) { ans += digits[i][j] * digits[j][i]; }
  cout << ans << endl;
}
