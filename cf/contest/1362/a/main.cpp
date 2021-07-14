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

i64 solve(i64 a, i64 b) {
  if (a == b) return 0;
  if (a > b) swap(a, b);
  if (b % a != 0) return -1;
  b /= a;
  int cnt = 0;
  while (b != 1) {
    if ((b & 1) == 1 && b != 1) return -1;
    b >>= 1;
    cnt++;
  }
  int n8 = cnt / 3;
  cnt %= 3;
  int n4 = cnt / 2;
  cnt %= 2;
  return n8 + n4 + cnt;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    i64 a, b;
    cin >> a >> b;
    cout << solve(a, b) << '\n';
  }
}
