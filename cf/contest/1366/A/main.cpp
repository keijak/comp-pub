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

i64 solve(i64 a, i64 b) {
  if (a > b) swap(a, b);
  if (b > 2 * a) {
    return a;
  }
  i64 r = min(b - a, a);
  b -= 2 * r;
  a -= r;
  r += 2 * (a / 3);
  a %= 3;
  b %= 3;
  if ((a >= 1 && b >= 2) || (a >= 2 && b >= 1)) {
    r += 1;
  }
  return r;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    i64 a, b;
    cin >> a >> b;
    cout << solve(a, b) << endl;
  }
}
