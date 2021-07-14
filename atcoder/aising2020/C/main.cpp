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
  vector<int> ans(N + 1);
  for (i64 x = 1; x <= N; ++x) {
    i64 x2 = x * x;
    for (i64 y = 1; y <= N; ++y) {
      i64 y2 = y * y;
      i64 xy = x * y;
      for (i64 z = 1;; ++z) {
        i64 a = x2 + y2 + z * z + xy + y * z + x * z;
        if (a > N) break;
        ans[a]++;
      }
    }
  }
  for (int i = 1; i <= N; ++i) {
    cout << ans[i] << '\n';
  }
}
