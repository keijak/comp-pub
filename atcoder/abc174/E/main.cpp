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
  i64 N, K;
  cin >> N >> K;
  vector<i64> A(N);
  i64 maxlen = 0;
  REP(i, N) {
    cin >> A[i];
    maxlen = max(maxlen, A[i]);
  }
  sort(A.begin(), A.end());
  auto is_ok = [&](double x) -> bool {
    assert(x > 0.0);
    i64 mincut = 0;
    REP(i, N) {
      if (A[i] <= x + 1e-8) {
        continue;
      }
      i64 c = ceil(A[i] / x) + 1e-8;
      c -= 1;
      mincut += c;
      if (mincut > K) return false;
    }
    return true;
  };
  double ng = 0, ok = maxlen;
  REP(iter, 200) {
    double mid = (ok + ng) / 2.0;
    if (is_ok(mid)) {
      ok = mid;
    } else {
      ng = mid;
    }
  }
  {
    i64 ans = ceil(ok) + 0.001;
    cout << ans << endl;
  }
}
