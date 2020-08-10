#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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

const u64 P = 200003;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  V<i64> A(N);
  REP(i, N) cin >> A[i];
  i64 asum = A[0];
  u64 ans = 0;
  for (int i = 1; i < N; ++i) {
    u64 r = (asum * A[i]) % P;
    ans += r;
    DEBUG(i, asum, A[i], r, ans);
    asum += A[i];
  }
  cout << ans << endl;
}
