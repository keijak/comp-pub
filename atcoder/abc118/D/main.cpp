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
  sort(A.rbegin(), A.rend());

  vector<int> dp(N + 1, -1);
  dp[0] = 0;
  REP(i, N) {
    if (dp[i] == -1) continue;
    REP(k, M) {
      int m = matches[A[k]];
      if (i + m > N) continue;
      dp[i + m] = max(dp[i + m], dp[i] + 1);
    }
  }
  assert(dp[N] >= 1);
  int s = N;
  REP(i, dp[N]) {
    REP(k, M) {
      int m = matches[A[k]];
      if (s - m >= 0 && dp[s - m] == dp[s] - 1) {
        cout << A[k];
        s -= m;
        break;
      }
    }
  }
  cout << '\n';
}
