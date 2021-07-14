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
  i64 K;
  cin >> K;
  int i = 1;
  int m = 7;
  vector<bool> seen(K);
  for (;; ++i) {
    m %= K;
    if (m == 0) {
      cout << i << endl;
      return 0;
    }
    if (seen[m]) {
      cout << -1 << endl;
      return 0;
    }
    seen[m] = true;
    m = m * 10 + 7;
  }
}
