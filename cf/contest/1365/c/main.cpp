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

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<int> A(N), B(N);
  for (auto& x : A) cin >> x;
  for (auto& x : B) cin >> x;

  vector<int> AX(N);
  REP(i, N) { AX[A[i] - 1] = i; }

  vector<int> shifts(N, 0);
  REP(i, N) {
    int ax = AX[B[i] - 1];
    int shift = (ax - i + N) % N;
    shifts[shift]++;
  }
  int ans = 0;
  REP(i, N) ans = max(ans, shifts[i]);
  cout << ans << endl;
}
