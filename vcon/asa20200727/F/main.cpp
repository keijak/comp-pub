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
  i64 H, W, N;
  cin >> H >> W >> N;

  map<tuple<i64, i64>, int> mp;
  REP(i, N) {
    i64 a, b;
    cin >> a >> b;
    a--;
    b--;
    for (int r = a - 1; r <= a + 1; ++r) {
      if (r <= 0 || r >= H - 1) continue;
      for (int c = b - 1; c <= b + 1; ++c) {
        if (c <= 0 || c >= W - 1) continue;
        mp[{r, c}]++;
      }
    }
  }
  vector<i64> ans(10);
  i64 total = 0;
  for (auto it = mp.begin(); it != mp.end(); ++it) {
    int cnt = it->second;
    assert(cnt > 0);
    assert(cnt < 10);
    ans[cnt]++;
    total++;
  }
  ans[0] = (H - 2) * (W - 2) - total;
  REP(i, 10) { cout << ans[i] << '\n'; }
}
