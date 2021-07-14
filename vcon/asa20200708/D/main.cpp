#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N;
  cin >> N;
  i64 ans = (N >= 3) ? N - 1 : 0;
  for (i64 k = 2; k * k <= N; ++k) {
    if (N % k == 0) {
      i64 r = N / k;
      assert(r >= k);
      DEBUG(N, k, r);
      i64 m = k - 1;
      if (N % m == r && r == N / m) {
        ans += m;
      }
      if (r - 1 != m) {
        m = r - 1;
        if (N % m == k && k == N / m) {
          ans += m;
        }
      }
    }
  }
  cout << ans << endl;
}
