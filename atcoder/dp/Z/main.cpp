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

const i64 INF = 1e8;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  i64 C;
  cin >> N >> C;
  vector<int> H(N);
  REP(i, N) cin >> H[i];
  int start = H[0];
  REP(i, N) H[i] -= start;
  start = 0;
  int goal = H[N - 1];
  if (goal < 0) {
    goal *= -1;
    REP(i, N) H[i] *= -1;
  }
  set<i64> hs;
  REP(i, N) {
    if (H[i] >= start && H[i] <= goal) {
      hs.insert(H[i]);
    }
  }
  vector<i64> hv(hs.begin(), hs.end());
  const int m = hv.size();

  vector<i64> dp(m);
  dp[m - 1] = 0;
  map<i64, int> jumpto;
  jumpto[hv[m - 1]] = m - 1;  // if less than x, go to y.
  for (int i = m - 2; i >= 0; --i) {
    auto it = jumpto.upper_bound(hv[i]);
    assert(it != jumpto.end());
    int j = it->second;
    assert(hv[j] > hv[i]);
    i64 hd = (hv[j] - hv[i]);
    dp[i] = hd * hd + C + dp[j];
    DEBUG(i, hv[i], j, hv[j], dp[i]);

    // ok-> big jump, ng -> small jump
    i64 ok = hv[i + 1], ng = -1e6;
    while (ok - ng > 1) {
      i64 mid = (ok + ng) / 2;
      i64 d1 = (hv[i + 1] - mid), d2 = (hv[i] - mid);
      if (d1 * d1 + dp[i + 1] <= d2 * d2 + dp[i]) {
        ok = mid;
      } else {
        ng = mid;
      }
    }
    DEBUG(ok, i);
    if (ok >= 0) {
      jumpto[ok] = i;
    }
  }
  cout << dp[0] << endl;
}
