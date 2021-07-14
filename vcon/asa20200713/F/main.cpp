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
  int M, K;
  cin >> M >> K;
  if (K == 0) {
    REP(i, 1LL << M) {
      if (i > 0) cout << " ";
      cout << i << " " << i;
    }
    cout << endl;
    return 0;
  }

  if (M == 1 || K >= (1LL << M)) {
    cout << -1 << endl;
    return 0;
  }

  i64 s = 0;
  REP(i, 1LL << M) { s ^= i; }
  DEBUG(s);
  assert(s == 0);
  vector<int> ans;
  ans.reserve(1LL << (M + 1));
  REP(i, 1LL << M) {
    if (i != K) ans.push_back(i);
  }
  ans.push_back(K);
  for (int i = (1LL << M) - 1; i >= 0; --i) {
    if (i != K) ans.push_back(i);
  }
  ans.push_back(K);
  REP(i, ans.size()) { cout << (i == 0 ? "" : " ") << ans[i]; }
  cout << endl;
}
