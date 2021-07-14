#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                \
  do {                            \
    cerr << " \033[33m (L";       \
    cerr << __LINE__ << ") ";     \
    cerr << #__VA_ARGS__ << ": "; \
    cerr << "\033[0m";            \
    debug(__VA_ARGS__);           \
    cerr << endl;                 \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

const u64 INF = numeric_limits<u64>::max();

u64 solve(int n, int k, int d) {
  u64 x = d;
  int dsum = 0;
  bool carry = false;
  REP(i, k + 1) {
    int dd = d + i;
    if (dd >= 10) {
      dd -= 9;
      carry = true;
    }
    dsum += dd;
  }
  if (dsum > n) return INF;
  if (dsum == n) return x;
  if (carry) {
    x += 80;
    dsum += 8 * (k + 1);
  } else {
    x += 90;
    dsum += 9 * (k + 1);
  }
  if (dsum > n) return INF;
  if (dsum == n) return x;
  if ((n - dsum) % (k + 1) != 0) return INF;
  int t = (n - dsum) / (k + 1);
  int r = t / 9;
  u64 base = 100;
  REP(i, r) {
    t -= 9;
    x += 9 * base;
    base *= 10;
  }
  assert(t >= 0);
  assert(t < 9);
  if (t > 0) {
    x += t * base;
  }
  return x;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int n, k;
    cin >> n >> k;
    u64 ans = INF;
    REP(d, 10) {
      u64 x = solve(n, k, d);
      ans = min(ans, x);
    }
    if (ans == INF) {
      cout << -1 << '\n';
    } else {
      cout << ans << '\n';
    }
  }
}
