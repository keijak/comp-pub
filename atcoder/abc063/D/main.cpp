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
  i64 n, a, b;
  cin >> n >> a >> b;
  vector<i64> h(n);
  i64 hmax = 0;
  REP(i, n) {
    cin >> h[i];
    hmax = max(hmax, h[i]);
  }

  i64 gap = a - b;
  auto check = [&](i64 cnt) -> bool {
    i64 acnt = 0;
    REP(i, n) {
      i64 rest = h[i] - b * cnt;
      if (rest <= 0) continue;
      acnt += (rest + gap - 1) / gap;
      if (acnt > cnt) return false;
    }
    return true;
  };

  i64 ng = -1, ok = hmax / b + 1;
  while (ok - ng > 1) {
    i64 mid = (ok + ng) / 2;
    if (check(mid)) {
      ok = mid;
    } else {
      ng = mid;
    }
  }
  cout << ok << endl;
}
