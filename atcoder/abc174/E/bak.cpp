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
  i64 N, K;
  cin >> N >> K;
  vector<i64> A(N);
  priority_queue<tuple<double, i64>> pq;
  REP(i, N) {
    cin >> A[i];
    pq.emplace(double(A[i]), 1);
  }
  i64 r = K;
  while (pq.size()) {
    auto [len, cnt] = pq.top();
    pq.pop();
    while (!pq.empty()) {
      auto [len2, cnt2] = pq.top();
      if (len2 + 1e-8 >= len) {
        cnt += cnt2;
        pq.pop();
      } else {
        break;
      }
    }
    i64 q = min(r, cnt);
    r -= q;
    cnt -= q;
    if (cnt > 0) {
      pq.emplace(len, cnt);
    }
    if (q > 0) {
      pq.emplace(len / 2.0, q);
    }
    if (r == 0) break;
  }
  {
    auto [len, _] = pq.top();
    i64 ans = ceil(len) + 0.001;
    cout << ans << endl;
  }
}
