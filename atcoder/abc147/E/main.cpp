#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

template <typename T>
using V = std::vector<T>;
template <typename T>
vector<T> make_vec(size_t n, T a) {
  return vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v) {
  os << '[';
  for (auto &&e : v) os << e << ' ';
  return os << ']';
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &v) {
  os << '[';
  for (auto &&e : v) os << e << ' ';
  return os << ']';
}

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
  int H, W;
  cin >> H >> W;
  auto A = make_vec(H, W, 0);
  auto B = make_vec(H, W, 0);
  REP(i, H) REP(j, W) cin >> A[i][j];
  REP(i, H) REP(j, W) cin >> B[i][j];

  const int M = 30000;
  auto dp = make_vec(H + 1, W + 1, M, false);
  dp[1][0][0] = dp[0][1][0] = true;
  for (int i = 1; i <= H; ++i) {
    for (int j = 1; j <= W; ++j) {
      const int delta = A[i - 1][j - 1] - B[i - 1][j - 1];
      REP(k, M) {
        int k1 = abs(k + delta);
        int k2 = abs(k - delta);
        if (dp[i - 1][j][k]) {
          assert(k1 < M && k2 < M);
          dp[i][j][k1] = true;
          dp[i][j][k2] = true;
        }
        if (dp[i][j - 1][k]) {
          assert(k1 < M && k2 < M);
          dp[i][j][k1] = true;
          dp[i][j][k2] = true;
        }
      }
    }
  }

  REP(k, M) {
    if (dp[H][W][k]) {
      cout << k << endl;
      return 0;
    }
  }
  assert(false);
}
