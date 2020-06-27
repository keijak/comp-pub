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
  int N;
  cin >> N;
  vector<i64> A(N);
  i64 ax = 0;
  REP(i, N) {
    cin >> A[i];
    if (i >= 2) ax ^= A[i];
  }
  auto solve = [&]() -> i64 {
    i64 ans = 0;
    i64 d = 1;
    i64 b = 0;
    for (; d <= A[0]; d <<= 1) {
      if ((A[0] ^ A[1]) == ax) return ans;

      i64 x = A[0] & d;
      i64 y = A[1] & d;
      i64 z = ax & d;

      if ((x ^ y) == z) {
        if (!z) {
          b = d;
        }
        continue;
      }
      assert((x ^ y) != z);
      if (b == 0) {
        DEBUG(d, A[0], A[1], ans);
        return -1;
      }
      A[0] -= b;
      A[1] += b;
      ans += b;
      if (!y) {
        b = 0;
      }
    }
    if ((A[0] ^ A[1]) == ax) return ans;
    return -1;
  };

  cout << solve() << endl;
}
