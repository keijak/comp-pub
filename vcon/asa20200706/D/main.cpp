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
  int N, K;
  cin >> N >> K;
  vector<vector<int>> T(N, vector<int>(K));
  REP(i, N) REP(j, K) { cin >> T[i][j]; }

  auto check = [&](auto rec, int i, int xors) -> bool {
    if (i == N) {
      return xors == 0;
    }
    REP(j, K) {
      if (rec(rec, i + 1, xors ^ T[i][j])) {
        return true;
      }
    }
    return false;
  };
  cout << (check(check, 0, 0) ? "Found" : "Nothing") << endl;
}
