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

i64 solve(const int N) {
  vector<int> A(N);
  for (auto& x : A) cin >> x;
  int bad_e = 0, bad_o = 0;
  REP(i, N) {
    if (i % 2 != A[i] % 2) {
      if (i % 2 == 1)
        ++bad_o;
      else
        ++bad_e;
    }
  }
  if (bad_o != bad_e) return -1;
  return bad_o;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int N;
    cin >> N;
    cout << solve(N) << '\n';
  }
}