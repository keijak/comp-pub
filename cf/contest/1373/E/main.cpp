#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef ENABLE_DEBUG
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

const i64 INF = 1LL << 61;

i64 s1(int n, int d) {
  if (n < d) return INF;
  i64 ans = d;
  n -= d;
  i64 base = 10;
  while (n >= 9) {
    ans += base * 9;
    base *= 10;
    n -= 9;
  }
  if (n > 0) {
    ans += base * n;
  }
  return ans;
}

i64 solve(int n, int k, int d) {
  if (k == 0) {
    return s1(n, d);
  }
  //
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  while (T--) {
    int n, k;
    cin >> n >> k;
    i64 ans = INF;
    REP(d, 10) {
      i64 x = 3;  // solve(n, k, d);
      DEBUG(d, x);
      ans = min(ans, x);
    }
    cout << (ans == INF) ? -1LL : ans << '\n';
  }
}
