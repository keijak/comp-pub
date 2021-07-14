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
  int n, m, x;
  cin >> n >> m >> x;
  V<int> cost(n);
  V<V<int>> A(n, V<int>(m));
  REP(i, n) {
    cin >> cost[i];
    REP(j, m) cin >> A[i][j];
  }
  i64 INF = 1LL << 50;
  i64 ans = INF;
  for (int s = 0; s < (1LL << n); ++s) {
    i64 csum = 0;
    V<int> comp(m);
    REP(i, n) {
      if (s & (1 << i)) {
        csum += cost[i];
        REP(j, m) { comp[j] += A[i][j]; }
      }
    }

    bool ok = true;
    REP(j, m) {
      if (comp[j] < x) {
        ok = false;
        break;
      }
    }
    if (ok) {
      ans = min(ans, csum);
    }
  }

  cout << (ans == INF ? -1LL : ans) << endl;
}
