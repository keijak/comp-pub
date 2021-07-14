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
  int N, K;
  cin >> N >> K;
  vector<pair<i64, i64>> ps(N);
  vector<i64> xs(N), ys(N);
  REP(i, N) {
    cin >> ps[i].first >> ps[i].second;
    xs[i] = ps[i].first;
    ys[i] = ps[i].second;
  }
  std::sort(ps.begin(), ps.end());
  std::sort(xs.begin(), xs.end());
  std::sort(ys.begin(), ys.end());
  i64 mina = numeric_limits<i64>::max();
  REP(xi, xs.size()) {
    REP(xj, xi) {
      REP(yi, ys.size()) {
        REP(yj, yi) {
          i64 ar = (xs[xi] - xs[xj]) * (ys[yi] - ys[yj]);
          int cnt = 0;
          REP(k, N) {
            auto [xk, yk] = ps[k];
            if (xk >= xs[xj] && xk <= xs[xi] && yk >= ys[yj] && yk <= ys[yi])
              ++cnt;
          }
          if (cnt >= K) {
            mina = min(mina, ar);
          }
        }
      }
    }
  }
  cout << (i64)(mina) << endl;
}
