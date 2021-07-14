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
  int N;
  cin >> N;
  // -1 => unknown, 1 => good, 0 => bad
  vector<vector<int>> p(N, vector<int>(N, -1));
  REP(i, N) {
    int a;
    cin >> a;
    REP(j, a) {
      int x, y;
      cin >> x >> y;
      x--;
      p[i][x] = y;
    }
  }
  vector<bool> good(N);
  auto ok = [&]() -> bool {
    REP(i, N) {
      if (good[i]) {
        REP(j, N) {
          if (p[i][j] != -1 && p[i][j] != good[j]) return false;
        }
      }
    }
    return true;
  };
  int max_good = 0;
  auto dfs = [&](auto rec, int i, int num) -> void {
    if (i == N) {
      if (ok()) {
        max_good = max(max_good, num);
      }
      return;
    }
    good[i] = false;
    rec(rec, i + 1, num);
    good[i] = true;
    rec(rec, i + 1, num + 1);
    good[i] = false;
  };
  dfs(dfs, 0, 0);
  cout << max_good << endl;
}
