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
  vector<i64> A(3 * N);
  for (auto& x : A) cin >> x;

  priority_queue<i64> q;
  vector<i64> lcum(N + 1);
  REP(i, N) {
    q.push(-A[i]);
    lcum[0] += A[i];
  }
  for (int i = 1; i <= N; ++i) {
    lcum[i] = lcum[i - 1] + A[N + i - 1];
    q.push(-A[N + i - 1]);
    i64 v = -q.top();
    lcum[i] -= v;
    q.pop();
  }

  priority_queue<i64> q2;
  vector<i64> rcum(N + 1);
  REP(i, N) {
    int j = 3 * N - 1 - i;
    q2.push(A[j]);
    rcum[N] += A[j];
  }
  for (int i = N - 1; i >= 0; --i) {
    rcum[i] = rcum[i + 1] + A[N + i];
    q2.push(A[N + i]);
    rcum[i] -= q2.top();
    q2.pop();
  }

  i64 ans = lcum[0] - rcum[0];
  REP(i, N + 1) { ans = max(ans, lcum[i] - rcum[i]); }
  cout << ans << endl;
}
