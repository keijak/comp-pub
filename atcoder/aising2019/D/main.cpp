//#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void _debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void _debug(T value, Ts... args) {
  std::cerr << value << ", ";
  _debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    _debug(__VA_ARGS__);               \
    cerr << endl;                      \
  } while (0)
#endif

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, Q;
  cin >> N >> Q;
  vector<i64> A(N);  // sorted
  for (auto& x : A) cin >> x;

  // rcum[i] = sum of A[i] and its right, inclusive.
  vector<i64> rcum(N + 1);
  REP(i, N) { rcum[N - 1 - i] = rcum[N - i] + A[N - 1 - i]; }

  // lhcum[i] = sum of A[i-2] and its left.
  vector<i64> lhcum(N + 2);
  for (int i = 2; i < N + 2; ++i) {
    lhcum[i] = lhcum[i - 2] + A[i - 2];
  }

  auto solve = [&](const i64 X) -> i64 {
    auto closest_it = lower_bound(A.begin(), A.end(), X);
    if (closest_it == A.end()) {
      return lhcum[N + 1];
    }
    int closest_i = closest_it - A.begin();
    if (closest_i > 0 && A[closest_i] - X >= X - A[closest_i - 1]) {
      --closest_i;
      --closest_it;
    }
    if (closest_i == N - 1) {
      return lhcum[N + 1];
    }
    DEBUG(closest_i, A[closest_i]);

    int ok = closest_i, ng = N - 1;
    while (ng - ok > 1) {
      int m = (ng + ok) / 2;
      i64 delta = abs(A[m] - X);
      int lb = lower_bound(A.begin(), A.end(), X - delta) - A.begin();
      bool b = m - lb + 1 > N - 1 - m;
      DEBUG(m, A[m], lb, A[lb], b);
      if (m - lb + 1 > N - 1 - m) {
        // m is too big
        ng = m;
      } else {
        ok = m;
      }
    }
    return rcum[ng] + lhcum[ok - (N - ng) + 2];
  };

  REP(i, Q) {
    i64 X;
    DEBUG(i);
    cin >> X;
    cout << solve(X) << '\n';
  }
}
