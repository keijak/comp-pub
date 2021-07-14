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
  string s;
  cin >> s;
  i64 ans = 0;
  auto solve = [&](auto rec, int l, int r, i64 acc) -> void {
    i64 x = 0;
    for (int i = l; i < r; ++i) {
      x = 10 * x + i64(s[i] - '0');
    }

    if (r == int(s.size())) {
      ans += acc + x;
      return;
    }

    rec(rec, l, r + 1, acc);

    rec(rec, r, r + 1, acc + x);
  };
  solve(solve, 0, 1, 0LL);
  cout << ans << '\n';
}
