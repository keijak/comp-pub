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

i64 solve(i64 n) {
  i64 even_to_odd = n / 2 + n % 2;
  DEBUG(even_to_odd);
  i64 ans = even_to_odd;

  for (i64 k = 1, p = 2; p <= n; ++k, p *= 2) {
    i64 cnt = n < p ? 0 : (1 + (n - p) / (2 * p));
    ans += (k + 1) * cnt;
    DEBUG(k, p, cnt, ans);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    i64 n;
    cin >> n;
    cout << solve(n) << '\n';
  }
}