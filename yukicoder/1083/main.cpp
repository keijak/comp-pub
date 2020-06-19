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

const i64 INF = 1LL << 40;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, K;
  cin >> N >> K;
  vector<i64> A(N);
  i64 mx = INF;
  for (auto& x : A) {
    cin >> x;
    mx = min(mx, x);
  }
  sort(A.rbegin(), A.rend());

  i64 ans = 0;
  auto solve = [&](auto rec, int i, i64 val) -> void {
    if (val < mx) {
      if (ans < val) ans = val;
      return;
    }
    if (i == N) {
      if (ans < val % mx) ans = val % mx;
      return;
    }

    rec(rec, i + 1, val);
    rec(rec, i + 1, val % A[i]);
  };
  solve(solve, 0, K);
  cout << ans << endl;
}