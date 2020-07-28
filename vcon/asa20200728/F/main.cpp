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
  int R, G, B;
  cin >> R >> G >> B;
  int ans = 1e8;
  for (int gh = 500; gh >= -500; --gh) {
    int gt = gh + G - 1;
    int cost2;
    if (gh >= 0) {
      cost2 = G * (2 * gh + G - 1) / 2;
    } else if (gt <= 0) {
      cost2 = G * (2 * -gt + G - 1) / 2;
    } else {
      int r = gt;
      int l = G - r - 1;
      assert(r >= 0);
      assert(l >= 0);
      cost2 = l * (l + 1) / 2 + r * (r + 1) / 2;
    }

    int bh_min = 100 - (B - 1) / 2;
    int bh = max(gt + 1, bh_min);
    int bt = bh + B - 1;
    if (bt < 100) continue;
    int cost1;
    if (bh >= 100) {
      int k = bh - 100;
      cost1 = B * (2 * k + B - 1) / 2;
    } else {
      int l = 100 - bh;
      int r = B - l - 1;
      assert(l >= 0);
      assert(r >= 0);
      cost1 = l * (l + 1) / 2 + r * (r + 1) / 2;
    }

    int rt_max = -100 + (R - 1) / 2;
    int rt = min(gh - 1, rt_max);
    int rh = rt - R + 1;
    if (rh > -100) continue;
    int cost3;
    if (rt <= -100) {
      int k = -100 - rt;
      cost3 = R * (2 * k + R - 1) / 2;
    } else {
      int r = rt + 100;
      int l = R - r - 1;
      assert(r >= 0);
      assert(l >= 0);
      cost3 = r * (r + 1) / 2 + l * (l + 1) / 2;
    }

    int v = cost1 + cost2 + cost3;
    if (ans > v) {
      ans = v;
      DEBUG(v, rh, rt, cost3, gh, gt, cost2, bh, bt, cost1);
    }
  }
  cout << ans << endl;
}
