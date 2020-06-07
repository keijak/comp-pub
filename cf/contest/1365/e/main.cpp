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

i64 solve(const int N) {
  vector<i64> A(N);
  for (auto& x : A) cin >> x;

  if (N == 1) return A[0];
  if (N == 2) return (A[0] | A[1]);

  i64 ans = 0;
  REP(i, N) {
    REP(j, i) {
      i64 xy = A[i] | A[j];
      REP(k, j) {
        i64 t = xy | A[k];
        if (ans < t) {
          ans = t;
        }
      }
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  cout << solve(N);
}
