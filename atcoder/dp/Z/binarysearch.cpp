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

#define chmin(x, y) x = min(x, y)

const i64 INF = 1e8;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  i64 C;
  cin >> n >> C;
  vector<i64> H(n);
  REP(i, n) cin >> H[i];

  vector<i64> dp(n);
  vector<tuple<i64, i64, int>> lines;

  auto is_ok = [&](int i, i64 x) -> bool {
    auto [a1, b1, _1] = lines[i];
    auto [a2, b2, _2] = lines[i - 1];
    i64 y1 = a1 * x + b1;
    i64 y2 = a2 * x + b2;
    if (y1 > y2) return false;
    return true;
  };

  auto jumpto = [&](int i) -> int {
    i64 x = H[i];
    int ok = 0, ng = int(lines.size());
    while (ng - ok > 1) {
      int mid = (ok + ng) / 2;
      if (is_ok(mid, x)) {
        ok = mid;
      } else {
        ng = mid;
      }
    }
    return get<2>(lines[ok]);
  };

  auto add_line = [&](int i) -> void {
    i64 a = -2 * H[i];
    i64 b = H[i] * H[i] + dp[i];
    while (lines.size() > 1) {
      int m = lines.size();
      auto [a1, b1, _1] = lines[m - 1];
      auto [a2, b2, _2] = lines[m - 2];
      if ((a2 - a1) * (b - b2) + (b1 - b2) * (a - a2) < 0) {
        break;
      }
      lines.pop_back();
    }
    lines.emplace_back(a, b, i);
  };

  dp[n - 1] = 0;
  add_line(n - 1);

  for (int i = n - 2; i >= 0; --i) {
    int j = jumpto(i);
    assert(j < n);
    assert(j >= 0);
    assert(H[j] > H[i]);
    i64 hd = H[j] - H[i];
    dp[i] = hd * hd + C + dp[j];
    add_line(i);
  }
  cout << dp[0] << endl;
}
