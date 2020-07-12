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
  i64 N, D, A;
  cin >> N >> D >> A;
  vector<i64> X;
  vector<tuple<i64, i64>> P;
  REP(i, N) {
    i64 x, h;
    cin >> x >> h;
    P.emplace_back(x, h);
    X.emplace_back(x);
  }
  sort(X.begin(), X.end());
  sort(P.begin(), P.end());
  vector<i64> acum(N);
  i64 cnt = 0;
  REP(i, N) {
    auto [x, h] = P[i];
    if (i > 0) acum[i] += acum[i - 1];
    DEBUG(h, acum[i]);
    h -= acum[i];
    if (h > 0) {
      i64 bom = (h + A - 1) / A;
      cnt += bom;
      acum[i] += bom * A;
      i64 xend = x + 2 * D;
      int j = upper_bound(X.begin(), X.end(), xend) - X.begin();
      if (j < N) {
        acum[j] -= bom * A;
      }
      DEBUG(bom, cnt, xend, j);
    }
  }
  cout << cnt << endl;
}
