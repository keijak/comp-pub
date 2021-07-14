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
  int N, M;
  cin >> N >> M;
  V<V<int>> B(N, V<int>(M));
  V<V<int>> A(N, V<int>(M, 0));
  REP(i, N) REP(j, M) {
    char c;
    cin >> c;
    B[i][j] = c - '0';
  }
  REP(i, N - 1) {
    for (int j = 1; j < M - 1; ++j) {
      int x = B[i][j];
      if (x > 0) {
        A[i + 1][j] += x;
        B[i][j] -= x;
        B[i + 1][j - 1] -= x;
        B[i + 1][j + 1] -= x;
        if (i + 2 < N) B[i + 2][j] -= x;
      }
    }
  }
  REP(i, N) {
    REP(j, M) { cout << A[i][j]; }
    cout << '\n';
  }
}
