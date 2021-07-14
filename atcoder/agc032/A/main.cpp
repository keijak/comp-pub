#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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
  V<int> B(N);
  REP(i, N) { cin >> B[i]; }

  V<int> ans(N, -1);
  REP(i, N) {
    for (int j = B.size() - 1; j >= 0; --j) {
      int x = B[j];
      int ins = x - 1;
      if (j < ins) {
        cout << -1 << endl;
        return 0;
      }
      if (j == ins) {
        ans[N - 1 - i] = x;
        B.erase(B.begin() + j);
        break;
      }
    }
  }
  REP(i, N) { cout << ans[i] << '\n'; }
}
