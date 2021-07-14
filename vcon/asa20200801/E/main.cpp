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
  vector<i64> A(N + 1);
  REP(i, N + 1) cin >> A[i];
  vector<i64> ub(N + 1);
  ub[0] = 1;
  for (int i = 1; i <= N; ++i) {
    if (i < 60) {
      ub[i] = ub[i - 1] * 2;
    } else {
      ub[i] = ub[i - 1];
    }
  }

  vector<i64> imax(N + 1), imin(N + 1);
  for (int i = N; i >= 0; --i) {
    // DEBUG(i, A[i], imin[i], imax[i], ub[i]);
    if (A[i] + imin[i] > ub[i]) {
      cout << -1 << endl;
      return 0;
    }
    if (i == 0) break;
    imin[i - 1] = (A[i] + imin[i] + 1) / 2;
    imax[i - 1] = min(A[i] + imax[i], ub[i - 1] - A[i - 1]);
  }

  i64 ans = 0;
  for (int i = 0; i <= N; ++i) {
    ans += imax[i] + A[i];
    DEBUG(i, imax[i], A[i], ans);
    if (i < N) {
      imax[i + 1] = min(imax[i + 1], imax[i] * 2 - A[i + 1]);
    }
  }
  cout << ans << endl;
}
