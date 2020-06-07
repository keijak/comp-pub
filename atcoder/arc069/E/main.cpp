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
  int N;
  cin >> N;
  vector<i64> A(N);
  vector<pair<i64, int>> S(N);
  i64 total = 0;
  REP(i, N) {
    cin >> A[i];
    S[i] = {A[i], -i};
    total += A[i];
  }
  DEBUG(total);
  sort(S.rbegin(), S.rend());
  vector<i64> ans(N);
  int pos = 0;
  i64 remaining = total;
  i64 prev_level = 0;
  while (remaining > 0) {
    DEBUG(pos, N - 1 - pos, remaining);
    int ix = -S[pos].second;
    if (ix == 0) {
      ans[ix] += remaining;
      remaining = 0;
      break;
    }
    int sm_pos = pos + 1;
    while (sm_pos < N && -S[sm_pos].second > ix) {
      sm_pos++;
    }
    assert(sm_pos != N);
    DEBUG(val, ix, sm_pos);
    i64 next_val = S[sm_pos].first;
    if (prev_level > next_val) {
      i64 d = (prev_level - next_val) * pos;
      ans[ix] += d;
      remaining -= d;
    }
    for (int i = pos; i < sm_pos; ++i) {
      i64 d = S[i].first - next_val;
      ans[ix] += d;
      remaining -= d;
      prev_level = next_val;
    }
    pos = sm_pos;
  }
  for (int x = 0; x < N; ++x) {
    cout << ans[x] << '\n';
  }
}
