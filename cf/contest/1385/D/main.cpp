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

i64 solve() {
  int n;
  cin >> n;
  string s;
  cin >> s;
  assert(int(s.size()) == n);

  auto min_move = [&](auto rec, int l, int r, int k) -> int {
    if (l + 1 == r) {
      if (s[l] - 'a' == k) {
        return 0;
      } else {
        return 1;
      }
    }
    int m = (l + r) / 2;
    int a1 = 0, a2 = 0;
    for (int i = l; i < m; ++i) {
      if (s[i] - 'a' != k) ++a1;
    }
    for (int i = m; i < r; ++i) {
      if (s[i] - 'a' != k) ++a2;
    }
    int x1 = a2 + rec(rec, l, m, k + 1);
    int x2 = a1 + rec(rec, m, r, k + 1);
    return min(x1, x2);
  };
  return min_move(min_move, 0, n, 0);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    cout << solve() << '\n';
  }
}
