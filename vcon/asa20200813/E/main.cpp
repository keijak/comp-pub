#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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
  int n, k;
  cin >> n >> k;
  V<pair<i64, i64>> points(n);
  V<i64> xs(n), ys(n);
  REP(i, n) {
    cin >> xs[i] >> ys[i];
    points[i] = {xs[i], ys[i]};
  }
  sort(xs.begin(), xs.end());
  sort(ys.begin(), ys.end());
  V<V<i64>> cum(n + 1, V<i64>(n + 1));
  for (auto [x, y] : points) {
    int xi = lower_bound(xs.begin(), xs.end(), x) - xs.begin();
    int yi = lower_bound(ys.begin(), ys.end(), y) - ys.begin();
    cum[xi + 1][yi + 1] += 1;
  }

  REP(i, n) {
    REP(j, n) {
      cum[i + 1][j + 1] += cum[i][j + 1] + cum[i + 1][j] - cum[i][j];
    }
  }

  //   REP(i, n + 1) {
  //     REP(j, n + 1) { cerr << (cum[i][j]) << ' '; }
  //     cerr << endl;
  //   }

  u64 ans = 1LL << 63;
  REP(i, n) REP(i1, i) REP(j, n) REP(j1, j) {
    i64 s = cum[i + 1][j + 1] - cum[i + 1][j1] - cum[i1][j + 1] + cum[i1][j1];
    u64 area = (xs[i] - xs[i1]) * (ys[j] - ys[j1]);
    DEBUG(s, area);
    if (s >= k) {
      ans = min(ans, area);
    }
  }
  cout << ans << endl;
}
