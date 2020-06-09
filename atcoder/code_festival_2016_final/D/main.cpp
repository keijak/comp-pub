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

const int L = 100'001;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  vector<int> X(N);
  for (auto& x : X) cin >> x;

  vector<int> counter(L);
  for (auto x : X) {
    ++counter[x];
  }

  vector<int> pair_cnt(M), rem_cnt(M);
  REP(x, L) {
    int cx = counter[x];
    pair_cnt[x % M] += (cx / 2) * 2;
    rem_cnt[x % M] += cx % 2;
  }

  i64 ans = 0;
  for (int i = 1; i <= M / 2; ++i) {
    int j = M - i;
    if (i >= j) break;
    int mr = min(rem_cnt[i], rem_cnt[j]);
    ans += mr;
    DEBUG(i, j, rem_cnt[i], rem_cnt[j], mr, ans);
    rem_cnt[i] -= mr;
    rem_cnt[j] -= mr;
    if (rem_cnt[i] > 0) {
      mr = min(rem_cnt[i], pair_cnt[j]);
      ans += mr;
      DEBUG(i, j, rem_cnt[i], pair_cnt[j], mr, ans);
      rem_cnt[i] -= mr;
      pair_cnt[j] -= mr;
    }
    if (rem_cnt[j] > 0) {
      mr = min(rem_cnt[j], pair_cnt[i]);
      ans += mr;
      DEBUG(i, j, pair_cnt[i], rem_cnt[j], mr, ans);
      rem_cnt[j] -= mr;
      pair_cnt[i] -= mr;
    }
    ans += pair_cnt[i] / 2;
    ans += pair_cnt[j] / 2;
    pair_cnt[i] %= 2;
    pair_cnt[j] %= 2;
  }
  ans += rem_cnt[0] / 2 + pair_cnt[0] / 2;
  if (M % 2 == 0) {
    int k = M / 2;
    ans += rem_cnt[k] / 2 + pair_cnt[k] / 2;
  }
  cout << ans << endl;
}
