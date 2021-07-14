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
  vector<int> P(N);
  for (auto& x : P) cin >> x;
  vector<i64> A(N), B(N);
  REP(i, N) { A[i] = 1LL + i * 20000LL; }
  REP(i, N) { B[N - 1 - i] = 1LL + i * 20000LL; }
  i64 prev = 0;
  REP(i, N) {
    int p = P[i] - 1;
    i64 v = A[p] + B[p];
    if (v <= prev) {
      i64 gap = prev - v;
      A[p] += gap / 2 + 1;
      B[p] += gap / 2 + 1;
      prev = A[p] + B[p];
    } else {
      prev = v;
    }
  }
  REP(i, N) { cout << (i == 0 ? "" : " ") << A[i]; }
  cout << endl;
  REP(i, N) { cout << (i == 0 ? "" : " ") << B[i]; }
  cout << endl;
}
