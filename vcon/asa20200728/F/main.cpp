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

int cost(int head, int tail, int num, int init) {
  if (head >= init) {
    int k = head - init;
    return num * (2 * k + num - 1) / 2;
  }
  if (tail <= init) {
    int k = init - tail;
    return num * (2 * k + num - 1) / 2;
  }
  int r = tail - init;
  int l = init - head;
  assert(r >= 0);
  assert(l >= 0);
  assert(l + r + 1 == num);
  return l * (l + 1) / 2 + r * (r + 1) / 2;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int R, G, B;
  cin >> R >> G >> B;
  int ans = 1e8;
  for (int gh = -500; gh <= 200; ++gh) {
    int gt = gh + G - 1;
    int cost1 = cost(gh, gt, G, 0);

    int bh_min = 100 - (B - 1) / 2;
    int bh = max(gt + 1, bh_min);
    int bt = bh + B - 1;
    int cost2 = cost(bh, bt, B, 100);

    int rt_max = -100 + (R - 1) / 2;
    int rt = min(gh - 1, rt_max);
    int rh = rt - R + 1;
    int cost3 = cost(rh, rt, R, -100);

    ans = min(ans, cost1 + cost2 + cost3);
  }
  cout << ans << endl;
}
