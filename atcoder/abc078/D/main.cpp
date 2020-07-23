#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
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

const i64 INF = 1LL << 40;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N, Z, W;
  cin >> N >> Z >> W;
  vector<i64> A(N);
  REP(i, N) cin >> A[i];

  i64 res = abs(A[N - 1] - W);
  if (N > 1) {
    res = max(res, abs(A[N - 1] - A[N - 2]));
  }
  cout << res << endl;
}
