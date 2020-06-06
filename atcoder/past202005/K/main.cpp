#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#endif

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, Q;
  cin >> N >> Q;

  vector<int> desktop(N), below(N, -1);
  REP(i, N) { desktop[i] = i; }

  REP(q, Q) {
    int f, t, x;
    cin >> f >> t >> x;
    f--;
    t--;
    x--;
    assert(f != t);
    int ftop = desktop[f];
    int ttop = desktop[t];
    int belowx = below[x];
    desktop[t] = ftop;
    desktop[f] = belowx;
    below[x] = ttop;
  }

  vector<int> ans(N);
  REP(i, N) {
    for (int k = desktop[i]; k >= 0; k = below[k]) {
      ans[k] = i + 1;
    }
  }
  REP(i, N) { cout << ans[i] << '\n'; }
}
