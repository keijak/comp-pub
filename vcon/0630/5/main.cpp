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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N, K;
  cin >> N >> K;
  vector<i64> A(N);
  for (auto& x : A) cin >> x;

  const i64 M = 1LL << 42;
  vector<i64> lower_sum(45), eq_sum(45);
  int k = 1;
  for (i64 d = M; d > 0; d >>= 1, ++k) {
    i64 cnt = 0;
    REP(i, N) {
      if (A[i] & d) cnt++;
    }
    if (lower_sum[k - 1] != 0) {
      lower_sum[k] = lower_sum[k - 1] + max(cnt, N - cnt) * d;
    }
    if (K & d) {
      eq_sum[k] = eq_sum[k - 1] + (N - cnt) * d;
      lower_sum[k] = max(lower_sum[k], eq_sum[k - 1] + cnt * d);
    } else {
      eq_sum[k] = eq_sum[k - 1] + cnt * d;
    }
  }
  i64 ans = max(lower_sum[k - 1], eq_sum[k - 1]);
  cout << ans << endl;
}
