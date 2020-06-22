#define ENABLE_DEBUG 1
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if ENABLE_DEBUG
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

const i64 INF = 1LL << 60;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N;
  cin >> N;
  vector<i64> A(3 * N);
  for (auto& x : A) cin >> x;
  i64 lsum = 0, rsum = 0;
  priority_queue<i64> lq, rq;
  REP(i, N) {
    lsum += A[i];
    lq.push(-A[i]);
    rsum += A[3 * N - 1 - i];
    rq.push(A[3 * N - 1 - i]);
  }
  vector<i64> lcum(N + 1), rcum(N + 1);
  rcum[N] = rsum;
  REP(i, N) {
    i64 r2 = A[2 * N - 1 - i];
    rq.push(r2);
    i64 r = rq.top();
    rq.pop();
    rcum[N - 1 - i] = rcum[N - i] - r + r2;
  }
  lcum[0] = lsum;
  REP(i, N) {
    i64 l2 = A[N + i];
    lq.push(-l2);
    i64 l = -lq.top();
    lq.pop();
    lcum[i + 1] = lcum[i] - l + l2;
  }
  i64 ans = -INF;
  for (int i = 0; i <= N; ++i) {
    ans = max(ans, lcum[i] - rcum[i]);
  }
  cout << ans << '\n';
}
