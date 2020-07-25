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
  int n;
  cin >> n;
  vector<i64> a(n), b(n);
  i64 asum = 0, bsum = 0;
  REP(i, n) {
    cin >> a[i];
    asum += a[i];
  }
  REP(i, n) {
    cin >> b[i];
    bsum += b[i];
  }
  auto check = [&]() -> bool {
    if (asum > bsum) return false;
    i64 count = bsum - asum;
    DEBUG(count, bsum, asum);
    i64 ca = 0, cb = 0;
    REP(i, n) {
      if (a[i] < b[i]) {
        ca += (b[i] - a[i] + 1) / 2;
        if (a[i] % 2 != b[i] % 2) ++cb;
      }
      if (a[i] > b[i]) {
        cb += a[i] - b[i];
      }
    }
    return ca <= count && cb <= count;
  };

  cout << (check() ? "Yes\n" : "No\n");
}
