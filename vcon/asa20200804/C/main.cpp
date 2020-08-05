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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  V<i64> A(N), B(N);
  i64 asum = 0, bsum = 0;
  REP(i, N) {
    cin >> A[i];
    asum += A[i];
  }
  REP(i, N) {
    cin >> B[i];
    bsum += B[i];
  }
  if (bsum > asum) {
    cout << -1 << endl;
    return 0;
  }
  V<tuple<i64, int>> dif(N);
  REP(i, N) { dif[i] = {A[i] - B[i], i}; }
  sort(dif.begin(), dif.end());
  int ans = 0;
  int hd = 0, tl = N - 1;
  for (; hd < N;) {
    DEBUG(hd, tl);
    auto& [dh, ih] = dif[hd];
    if (dh >= 0) break;
    if (tl < 0) {
      cout << -1 << endl;
      return 0;
    }
    auto& [dt, it] = dif[tl];
    i64 x = min(-dh, dt);
    dh += x;
    dt -= x;
    if (dh == 0) ++hd;
    if (dt == 0) --tl;
  }
  REP(i, N) {
    auto [x, y] = dif[i];
    if (x != A[y] - B[y]) {
      ++ans;
    }
  }
  cout << ans << endl;
}
