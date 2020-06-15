#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define debug(...)
#define DEBUG(...)
#else
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 M, N;
  cin >> M >> N;
  if (N > M) swap(N, M);
  if (M * N <= 10) {
    vector<vector<int>> board(N, vector<int>(M));
    auto dfs = [&](auto rec, int x) -> i64 {
      if (x == M * N) {
        return 1;
      }
      i64 ans = 0;
      int i = x / M, j = x % M;
      for (int k = 1; k <= 3; ++k) {
        if (k == 1 && i > 0 && board[i - 1][j] == 1) continue;
        if (k == 1 && j > 0 && board[i][j - 1] == 1) continue;

        if (k == 2 && i > 0 && board[i - 1][j] == 2) continue;
        if (k == 2 && i > 1 && board[i - 2][j] == 2) continue;
        if (k == 2 && j > 0 && board[i][j - 1] == 2) continue;
        if (k == 2 && j > 1 && board[i][j - 2] == 2) continue;

        if (k == 3 && i > 0 && board[i - 1][j] == 3) continue;
        if (k == 3 && i > 1 && board[i - 2][j] == 3) continue;
        if (k == 3 && i > 2 && board[i - 3][j] == 3) continue;
        if (k == 3 && j > 0 && board[i][j - 1] == 3) continue;
        if (k == 3 && j > 1 && board[i][j - 2] == 3) continue;
        if (k == 3 && j > 2 && board[i][j - 3] == 3) continue;
        board[i][j] = k;
        ans += rec(rec, x + 1);
      }
      return ans;
    };
    cout << dfs(dfs, 0) << endl;
  } else if (M >= 3 && N >= 3) {
    if ((M + N) % 2 == 0) cout << 18 << endl;
    if ((M + N) % 4 == 1) cout << 20 << endl;
    if ((M + N) % 4 == 3) cout << 16 << endl;
  } else if (N == 2) {
    if (M % 2 == 0) cout << 18 << endl;
    if (M % 4 == 1) cout << 16 << endl;
    if (M % 4 == 3) cout << 20 << endl;
  } else {
    assert(N == 1);
    if (M % 4 == 0) cout << 10 << endl;
    if (M % 4 == 1) cout << 9 << endl;
    if (M % 4 == 2) cout << 8 << endl;
    if (M % 4 == 3) cout << 9 << endl;
  }
}
