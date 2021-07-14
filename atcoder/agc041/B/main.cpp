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
  i64 N, M, v, P;
  cin >> N >> M >> v >> P;
  V<i64> A(N);
  REP(i, N) cin >> A[i];
  sort(A.rbegin(), A.rend());
  i64 pth = A[P - 1];

  auto check = [&](int x) -> bool {
    --x;
    if (x < P) return true;
    if (x > N - 1) return false;
    i64 cnt1 = (P - 1) + (N - x);
    i64 h = A[x] + M;
    if (h < pth) return false;
    if (cnt1 >= v) return true;
    i64 room = 0;
    for (int i = P - 1; i < x; ++i) {
      room += h - A[i];
    }
    return (cnt1 * M + room >= M * v);
  };

  i64 ok = P, ng = N + 1;
  while (ng - ok > 1) {
    i64 mid = (ok + ng) / 2;
    if (check(mid)) {
      ok = mid;
    } else {
      ng = mid;
    }
  }
  cout << ok << endl;
}
