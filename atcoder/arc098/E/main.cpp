//#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void _debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void _debug(T value, Ts... args) {
  std::cerr << value << ", ";
  _debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    _debug(__VA_ARGS__);               \
    cerr << endl;                      \
  } while (0)
#endif

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, K, Q;
  cin >> N >> K >> Q;
  vector<i64> A(N);
  for (auto& x : A) cin >> x;
  vector<pair<i64, int>> p(N);
  REP(i, N) p[i] = make_pair(A[i], i);
  sort(p.begin(), p.end());
  i64 low = p[0].first;
  int lowi = 0;
  i64 ans = p[Q - 1].first - low;
  DEBUG(low, p[Q - 1].first, ans);

  vector<i64> taken, buf;
  taken.reserve(N);
  buf.reserve(N);
  for (;;) {
    while (lowi < N && p[lowi].first == low) {
      int j = p[lowi].second;
      A[j] = 0;
      lowi++;
    }
    if (lowi == N) break;
    low = p[lowi].first;
    taken.clear();
    for (int start = 0; start + K - 1 < N; ++start) {
      if (A[start] == 0) {
        continue;
      }
      bool failed = false;
      buf.clear();
      REP(i, K) {
        if (A[start + i] == 0) {
          failed = true;
          start = start + i;
          break;
        }
        buf.push_back(A[start + i]);
      }
      if (failed) {
        continue;
      }
      int take_cnt = 1;
      start += K;
      for (; start < N; ++start) {
        if (A[start] == 0) {
          break;
        }
        buf.push_back(A[start]);
        take_cnt++;
      }
      partial_sort(buf.begin(), buf.begin() + take_cnt, buf.end());
      REP(i, take_cnt) { taken.push_back(buf[i]); }
    }
    if (taken.size() < (size_t)Q) continue;
    sort(taken.begin(), taken.end());
    if (taken.front() != low) continue;

    ans = min(ans, taken[Q - 1] - low);
    DEBUG(low, taken[Q - 1], ans);
  }

  cout << ans << endl;
}
