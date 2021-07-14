#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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

  i64 n, a, b, c, d;
  cin >> n >> a >> b >> c >> d;
  assert(c <= d);

  const i64 x = (a < b) ? (b - a) : (a - b);

  const bool ok = [&]() -> bool {
    i64 high = (n - 1) * d;
    i64 low = (n - 1) * c;

    // Go up i times, go down (n-1-i) times.
    for (int i = n - 1; i >= 0; --i) {
      if (x > high || high < 0) return false;
      if (low <= x && x <= high) return true;
      high -= c + d;
      low -= c + d;
    }
    return false;
  }();
  cout << (ok ? "YES" : "NO") << endl;
}
