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
  int n, m;
  cin >> n >> m;
  vector<string> A(n), B(m);
  REP(i, n) { cin >> A[i]; }
  REP(i, m) { cin >> B[i]; }
  bool ok = [&]() {
    REP(r, n - m + 1) {
      REP(c, n - m + 1) {
        REP(i, m) REP(j, m) {
          if (B[i][j] != A[r + i][c + j]) {
            goto no_match;
          }
        }
        return true;
      no_match:
        continue;
      }
    }
    return false;
  }();
  cout << (ok ? "Yes\n" : "No\n");
}
