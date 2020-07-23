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
  i64 N, K, M;
  cin >> N >> K >> M;
  vector<i64> A(N);
  for (auto& x : A) cin >> x;
  vector<i64> acum(N + 1), abscum(N + 1);
  REP(i, N) {
    acum[i + 1] = acum[i] + A[i];
    abscum[i + 1] = abscum[i] + abs(A[i]);
  }

  vector<vector<i64>> memo(N + 1, vector<i64>(K + 1, -1));
  auto f = [&](auto rec, int len, int block_count) -> i64 {
    if (len == 0 || block_count == 0) return 0;
    if (len < block_count) return 0;
    if (memo[len][block_count] >= 0) return memo[len][block_count];
    if (len == block_count) {
      return (memo[len][block_count] = abscum[len]);
    }
    if (block_count == 1) {
      return (memo[len][block_count] = abs(acum[len]));
    }
    int m = min<int>(M, len);
    i64 max_score = 0;
    i64 block_sum = 0;
    for (int x = 1; x <= m; ++x) {
      block_sum += A[len - x];
      i64 r = rec(rec, len - x, block_count - 1);
      max_score = max(max_score, r + abs(block_sum));
    }
    return (memo[len][block_count] = max_score);
  };
  i64 ans = f(f, N, K);
  cout << ans << endl;
}
