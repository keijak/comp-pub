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

const int matches[10] = {-1, 2, 5, 5, 4, 5, 6, 3, 7, 6};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;
  vector<int> A(M);
  for (auto& x : A) cin >> x;
  sort(A.begin(), A.end());

  vector<vector<int>> dp(N / 2 + 1, vector<int>(N + 1, -1));
  dp[0][0] = 0;
  REP(i, N / 2 + 2) {
    bool placed = false;
    REP(j, N + 1) {
      if (dp[i][j] < 0) continue;
      REP(k, M) {
        int m = matches[A[k]];
        if (j + m > N) continue;
        dp[i + 1][j + m] = max(dp[i + 1][j + m], A[k]);
        placed = true;
      }
    }
    if (!placed) {
      break;
    }
  }
  int mi = -1;
  for (int i = N / 2; i >= 0; --i) {
    if (dp[i][N] >= 0) {
      mi = i;
      break;
    }
  }
  assert(mi >= 0);
  int s = N;
  for (int i = mi; i > 0; --i) {
    int d = dp[i][s];
    cout << d;
    s -= matches[d];
  }
  cout << '\n';
}