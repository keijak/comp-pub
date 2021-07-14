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
  int N;
  cin >> N;
  vector<i64> ans(N + 1);
  for (i64 x = 1; x * x <= N; ++x) {
    for (i64 y = 1; x * x + y * y + x * y <= N; ++y) {
      for (i64 z = 1;; ++z) {
        i64 s = x * x + y * y + z * z + x * y + y * z + z * x;
        if (s > N) break;
        ans[s]++;
      }
    }
  }
  for (int i = 1; i <= N; ++i) {
    cout << ans[i] << '\n';
  }
}
