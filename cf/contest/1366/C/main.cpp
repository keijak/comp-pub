//#define DEBUGGING  // Enables DEBUG macro.
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

i64 solve(int n, int m, vector<vector<int>>& A) {
  vector<int> v(n + m);
  REP(i, n) REP(j, m) {
    if (2 * (i + j + 1) >= (n + m)) {
      continue;
    }
    if (A[i][j] == 1) {
      v[i + j]++;
    } else {
      v[i + j]--;
    }
    if (A[n - 1 - i][m - 1 - j] == 1) {
      v[i + j]++;
    } else {
      v[i + j]--;
    }
  }
  i64 ans = 0;
  REP(i, n) REP(j, m) {
    int c = (v[i + j] >= 0) ? 1 : 0;
    if (2 * (i + j + 1) >= (n + m)) {
      continue;
    }
    DEBUG(i, j, v[i + j], c);
    if (A[i][j] != c) ans++;
    if (A[n - 1 - i][m - 1 - j] != c) ans++;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    DEBUG(T, "===");
    int n, m;
    cin >> n >> m;
    vector<vector<int>> A(n, vector<int>(m));
    REP(i, n) REP(j, m) cin >> A[i][j];
    cout << solve(n, m, A) << '\n';
  }
}