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
  auto solve = [&](int n) {
    int a = 2 * 3, b = 2 * 5, c = 2 * 7, d = 3 * 5;
    int x = n - a - b - c;
    if (x > 0 && x != a && x != b && x != c) {
      cout << "YES\n";
      cout << a << ' ' << b << ' ' << c << ' ' << x << '\n';
      return;
    }
    x = n - a - b - d;
    if (x > 0 && x != a && x != b && x != d) {
      cout << "YES\n";
      cout << a << ' ' << b << ' ' << d << ' ' << x << '\n';
      return;
    }
    x = n - b - c - d;
    if (x > 0 && x != c && x != b && x != d) {
      cout << "YES\n";
      cout << c << ' ' << b << ' ' << d << ' ' << x << '\n';
      return;
    }
    x = n - a - c - d;
    if (x > 0 && x != c && x != a && x != d) {
      cout << "YES\n";
      cout << c << ' ' << a << ' ' << d << ' ' << x << '\n';
      return;
    }
    { cout << "NO\n"; }
  };

  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    solve(n);
  }
}
