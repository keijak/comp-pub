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
  i64 H, W;
  cin >> H >> W;
  const i64 INF = 1LL << 50;
  i64 ans = INF;
  if (H % 3 == 0 || W % 3 == 0) {
    ans = 0;
  } else {
    ans = min(H, W);
  }

  for (int i = 1; i <= H / 2; ++i) {
    i64 s1 = i * W;
    i64 s2 = (H - i) * (W / 2);
    i64 s3 = (H - i) * ((W + 1) / 2);
    i64 g = max({s1, s2, s3}) - min({s1, s2, s3});
    ans = min(ans, g);
  }
  for (int i = 1; i <= W / 2; ++i) {
    i64 s1 = i * H;
    i64 s2 = (W - i) * (H / 2);
    i64 s3 = (W - i) * ((H + 1) / 2);
    i64 g = max({s1, s2, s3}) - min({s1, s2, s3});
    ans = min(ans, g);
  }
  cout << ans << endl;
}