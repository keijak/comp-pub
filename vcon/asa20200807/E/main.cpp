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

const i64 INF = 1LL << 40;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  V<i64> A(N);
  i64 mina = INF, maxa = -INF;
  int mini = -1, maxi = -1;
  REP(i, N) {
    cin >> A[i];
    if (mina > A[i]) {
      mina = A[i];
      mini = i;
    }
    if (maxa < A[i]) {
      maxa = A[i];
      maxi = i;
    }
  }

  V<pair<int, int>> ans;
  if (maxa > -mina) {
    REP(i, N) {
      if (A[i] < 0) {
        ans.emplace_back(maxi, i);
        A[i] += maxa;
      }
    }
    for (int i = 1; i < N; ++i) {
      if (A[i] < A[i - 1]) {
        ans.emplace_back(i - 1, i);
        A[i] += A[i - 1];
      }
    }

  } else {
    REP(i, N) {
      if (A[i] > 0) {
        ans.emplace_back(mini, i);
        A[i] += mina;
      }
    }
    for (int i = N - 2; i >= 0; --i) {
      if (A[i] > A[i + 1]) {
        ans.emplace_back(i + 1, i);
        A[i] += A[i + 1];
      }
    }
  }
  cout << ans.size() << endl;
  for (auto [x, y] : ans) {
    cout << x + 1 << ' ' << y + 1 << '\n';
  }
}
