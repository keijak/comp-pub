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
  int N;
  cin >> N;
  vector<i64> A(N);
  for (auto& x : A) cin >> x;
  sort(A.begin(), A.end());
  i64 amax = A[N - 1];
  vector<bool> non_divisible(amax + 1, true);
  REP(i, N - 1) {
    i64 x = A[i];
    if (!non_divisible[x]) continue;
    for (i64 y = 2 * x; y <= amax; y += x) {
      non_divisible[y] = false;
    }
  }
  REP(i, N - 1) {
    if (A[i] == A[i + 1]) {
      non_divisible[A[i]] = false;
    }
  }
  int ans = 0;
  REP(i, N) {
    if (non_divisible[A[i]]) ++ans;
  }
  cout << ans << endl;
}
