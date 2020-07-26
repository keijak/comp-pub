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
  string A;
  cin >> A;
  auto solve = [&]() -> i64 {
    int n = A.size();
    if (n == 1) return 0;

    vector<bool> sym(n);
    int sym_count = 0;
    for (int i = 0; i < n / 2; ++i) {
      sym[i] = sym[n - 1 - i] = (A[i] == A[n - 1 - i]);
      if (sym[i]) ++sym_count;
    }
    i64 ans = 0;
    for (int i = 0; i < n / 2; ++i) {
      if (sym[i]) {
        ans += 25 * 2;
      } else if (sym_count == n / 2 - 1) {
        ans += 24 * 2;
      } else {
        ans += 25 * 2;
      }
    }
    if (n % 2 == 1) {
      if (sym_count != n / 2) ans += 25;
    }
    return ans;
  };
  cout << solve() << '\n';
}
