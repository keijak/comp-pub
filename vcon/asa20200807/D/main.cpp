#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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
  int H, W, N;
  cin >> H >> W >> N;
  V<int> A(N);
  REP(i, N) cin >> A[i];
  V<V<int>> grid(H, V<int>(W, -1));
  int i = 0;
  REP(r, H) {
    REP(c, W) {
      int j = (r % 2 == 0) ? c : (W - 1 - c);
      while (A[i] == 0) ++i;
      grid[r][j] = i;
      A[i]--;
    }
  }
  REP(r, H) {
    REP(c, W) { cout << grid[r][c] + 1 << ' '; }
    cout << '\n';
  }
}
