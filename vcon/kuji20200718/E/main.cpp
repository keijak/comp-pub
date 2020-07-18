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
  int N, M;
  cin >> N >> M;
  string S;
  cin >> S;
  const int INF = 1 << 25;
  vector<int> steps(N + 1, INF);
  vector<int> nex(N + 1, -1);
  steps[N] = 0;
  int t = N;
  for (; t > 0;) {
    if (S[t] == '1') continue;
    int c = max(t - M, 0);
    int mint = t;
    for (int k = t - 1; k >= c; --k) {
      if (S[k] == '1') continue;
      steps[k] = steps[t] + 1;
      nex[k] = t;
      mint = k;
    }
    if (mint == t && t != 0) {
      cout << -1 << endl;
      return 0;
    }
    t = mint;
  }

  int x = 0;
  while (true) {
    int y = nex[x];
    if (y == -1) break;
    if (x != 0) cout << " ";
    cout << (y - x);
    x = y;
  }
  cout << endl;
}
